/*
 * Copyright 2022 The TensorFlow Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *             http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package uni.proj.rv.fragments

import android.annotation.SuppressLint
import android.app.Activity
import android.content.res.Configuration
import android.graphics.Bitmap
import android.graphics.Color
import android.graphics.Matrix
import android.graphics.RectF
import android.os.Bundle
import android.text.method.ScrollingMovementMethod
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.AdapterView
import android.widget.SeekBar
import android.widget.Spinner
import android.widget.Toast
import androidx.appcompat.widget.AppCompatSpinner
import androidx.camera.core.*
import androidx.camera.core.ImageAnalysis.OUTPUT_IMAGE_FORMAT_RGBA_8888
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.core.content.ContextCompat
import androidx.core.content.res.ResourcesCompat
import androidx.core.graphics.get
import androidx.core.graphics.set
import androidx.fragment.app.Fragment
import androidx.navigation.Navigation
import org.opencv.android.Utils
import org.opencv.core.Scalar
import org.tensorflow.lite.task.vision.detector.Detection
import uni.proj.ec.Command
import uni.proj.ec.SerialCommunication
import uni.proj.rv.General
import uni.proj.rv.ImageProcessing
import uni.proj.rv.ObjectDetectorHelper
import uni.proj.rv.R
import uni.proj.rv.RobotGame
import uni.proj.rv.databinding.FragmentCameraBinding
import uni.proj.rv.games.BallCollectingGame
import uni.proj.rv.games.DartsGame
import uni.proj.rv.games.DefendingGame
import uni.proj.rv.games.MazeGame
import uni.proj.rv.games.ShootingGame
import java.util.*
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors
import kotlin.collections.ArrayList
import kotlin.jvm.internal.Intrinsics.Kotlin
import kotlin.math.*


class CameraFragment : Fragment(), ObjectDetectorHelper.DetectorListener {

    private val TAG = "ObjectDetection"

    private var _fragmentCameraBinding: FragmentCameraBinding? = null

    private val fragmentCameraBinding
        get() = _fragmentCameraBinding!!

    private lateinit var objectDetectorHelper: ObjectDetectorHelper
    lateinit var bitmapBuffer: Bitmap
    lateinit var rotatedBuffer: Bitmap
    lateinit var hsvBuffer: Bitmap
    lateinit var detectionBuffer: Bitmap
    lateinit var previewBuffer: Bitmap

    private var preview: Preview? = null
    private var imageAnalyzer: ImageAnalysis? = null
    private var camera: Camera? = null
    private var cameraProvider: ProcessCameraProvider? = null
    private var tracking = false;
    var current_preview = 0;
    private var hsv_clipping = false
    private var bitmap_lock: Any = Object() //used in locking
    private var dectection_lock: Any = Object() //used in locking
    private var detectionResult: LinkedList<RectF> = LinkedList<RectF>()
    private var detectionWidth = 0
    private var detectionHeight = 0

    private var registeredGames: ArrayList<RobotGame> = ArrayList()
    private var currentGame = 0

    /** Blocking camera operations are performed using this executor */
    private lateinit var cameraExecutor: ExecutorService

    override fun onResume() {
        super.onResume()
        // Make sure that all permissions are still present, since the
        // user could have removed them while the app was in paused state.
        if (!PermissionsFragment.hasPermissions(requireContext())) {
            Navigation.findNavController(requireActivity(), R.id.fragment_container)
                .navigate(CameraFragmentDirections.actionCameraToPermissions())
        }
    }

    override fun onDestroyView() {
        _fragmentCameraBinding = null
        super.onDestroyView()

        // Shut down our background executor
        cameraExecutor.shutdown()
    }

    override fun onCreateView(
      inflater: LayoutInflater,
      container: ViewGroup?,
      savedInstanceState: Bundle?
    ): View {
        _fragmentCameraBinding = FragmentCameraBinding.inflate(inflater, container, false)

        General.communication = SerialCommunication(
            context as Activity?,
            object : SerialCommunication.SerialCommunicationListener {
                var cmd = ""
                var reading_cmd = false
                var text = ""

                override fun onConnected() {
                    Toast.makeText(context, "Connected", Toast.LENGTH_SHORT).show()
                }

                override fun onConnectionFailed(reason: Int) {
                    Toast.makeText(context, "Connection Failed , code: $reason", Toast.LENGTH_SHORT)
                        .show()
                }

                override fun onDisconnected() {
                    Toast.makeText(context, "Disconnected", Toast.LENGTH_SHORT).show()
                }

                override fun onPermissionFailed() {
                    Toast.makeText(context, "Permission Failed", Toast.LENGTH_SHORT).show()
                }

                @SuppressLint("SetTextI18n")
                override fun onDataReceived(data: ByteArray) {
                    var i = 0
                    while (data.size > i){
                        process(data[i].toInt().toChar())
                        i++
                    }
                    flush()
                }

                fun process(c : Char){
                    if (reading_cmd){
                        cmd += c
                        if (c == '}') {
                            reading_cmd = false
                            try {
                                //printSerial("[Running Command] $cmd")
                                registeredGames[currentGame].onCommand(Command.fromString(cmd))
                            }catch (e : java.lang.Exception){
                                e.printStackTrace()
                            }
                        }
                    }else{
                        if (c == '$') {
                            reading_cmd = true
                            cmd = ""
                            flush()
                            return
                        }
                        text += c
                    }
                }

                fun flush(){
                    if (text.isEmpty()) return
                    printSerial(text)
                    text = ""
                }

                override fun onDeviceAttached() {
                    General.communication.connect();
                    Toast.makeText(context, "Device Attached", Toast.LENGTH_SHORT).show()
                }

                override fun onDeviceDetached() {
                    //TODO: nothing ..
                }
            })

        return fragmentCameraBinding.root
    }

    @SuppressLint("MissingPermission")
    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        objectDetectorHelper = ObjectDetectorHelper(
            context = requireContext(),
            objectDetectorListener = this)

        // Initialize our background executor
        cameraExecutor = Executors.newSingleThreadExecutor()

        // Wait for the views to be properly laid out
        fragmentCameraBinding.viewFinder.post {
            // Set up the camera and its use cases
            setUpCamera()
        }

        // Attach listeners to UI control widgets
        initBottomSheetControls()
        initSerialControls()
        initTrackingControls()
        updatePreviewName()
        loadGames()
    }

    private fun loadGames(){
        //TODO: add your games here
        registeredGames.add(BallCollectingGame(this))
        registeredGames.add(DartsGame(this))
        registeredGames.add(DefendingGame(this))
        registeredGames.add(MazeGame(this))
        registeredGames.add(ShootingGame(this))

        registeredGames[currentGame].onGameSelected()

        fragmentCameraBinding.control.controlGameSelector.onItemSelectedListener = object : AdapterView.OnItemSelectedListener{
            override fun onItemSelected(
                parent: AdapterView<*>?,
                view: View?,
                position: Int,
                id: Long
            ) {
                registeredGames[currentGame].onGameNotSelected()
                currentGame = position
                registeredGames[currentGame].onGameSelected()
            }

            override fun onNothingSelected(parent: AdapterView<*>?) {
            }

        }
    }

    private fun updatePreviewName(){
        fragmentCameraBinding.control.controlPreviewName.text =
            when(current_preview){
                (0) -> "src"
                (1) -> "blur"
                (2) -> "hsv"
                (3) -> "mask"
                (4) -> "dilate"
                (5) -> "erode"
                (6) -> "blob"
                else -> {"ERROR"}
            }
    }

    private fun updateHSVtv(){
        fragmentCameraBinding.control.controlHsvView.text = String.format("[%d , %d , %d] > [%d , %d , %d]" ,
            fragmentCameraBinding.control.controlHsvHFrom.progress,
            fragmentCameraBinding.control.controlHsvSFrom.progress,
            fragmentCameraBinding.control.controlHsvVFrom.progress,
            fragmentCameraBinding.control.controlHsvHTo.progress,
            fragmentCameraBinding.control.controlHsvSTo.progress,
            fragmentCameraBinding.control.controlHsvVTo.progress,
        )
    }

    private fun initTrackingControls(){
        fragmentCameraBinding.control.controlHsvHFrom.setOnSeekBarChangeListener(
            object: SeekBar.OnSeekBarChangeListener {
                override fun onStartTrackingTouch(seekBar: SeekBar?) {
                }
                override fun onProgressChanged(
                    seekBar: SeekBar?,
                    progress: Int,
                    fromUser: Boolean
                ) {
                    updateHSVtv()
                }

                override fun onStopTrackingTouch(seekBar: SeekBar?) {
                }
            }
        )

        fragmentCameraBinding.control.controlHsvSFrom.setOnSeekBarChangeListener(
            object: SeekBar.OnSeekBarChangeListener {
                override fun onStartTrackingTouch(seekBar: SeekBar?) {
                }
                override fun onProgressChanged(
                    seekBar: SeekBar?,
                    progress: Int,
                    fromUser: Boolean
                ) {
                    updateHSVtv()
                }

                override fun onStopTrackingTouch(seekBar: SeekBar?) {
                }
            }
        )

        fragmentCameraBinding.control.controlHsvVFrom.setOnSeekBarChangeListener(
            object: SeekBar.OnSeekBarChangeListener {
                override fun onStartTrackingTouch(seekBar: SeekBar?) {
                }
                override fun onProgressChanged(
                    seekBar: SeekBar?,
                    progress: Int,
                    fromUser: Boolean
                ) {
                    updateHSVtv()
                }

                override fun onStopTrackingTouch(seekBar: SeekBar?) {
                }
            }
        )

        fragmentCameraBinding.control.controlHsvHTo.setOnSeekBarChangeListener(
            object: SeekBar.OnSeekBarChangeListener {
                override fun onStartTrackingTouch(seekBar: SeekBar?) {
                }
                override fun onProgressChanged(
                    seekBar: SeekBar?,
                    progress: Int,
                    fromUser: Boolean
                ) {
                    updateHSVtv()
                }

                override fun onStopTrackingTouch(seekBar: SeekBar?) {
                }
            }
        )

        fragmentCameraBinding.control.controlHsvSTo.setOnSeekBarChangeListener(
            object: SeekBar.OnSeekBarChangeListener {
                override fun onStartTrackingTouch(seekBar: SeekBar?) {
                }
                override fun onProgressChanged(
                    seekBar: SeekBar?,
                    progress: Int,
                    fromUser: Boolean
                ) {
                    updateHSVtv()
                }

                override fun onStopTrackingTouch(seekBar: SeekBar?) {
                }
            }
        )

        fragmentCameraBinding.control.controlHsvVTo.setOnSeekBarChangeListener(
            object: SeekBar.OnSeekBarChangeListener {
                override fun onStartTrackingTouch(seekBar: SeekBar?) {
                }
                override fun onProgressChanged(
                    seekBar: SeekBar?,
                    progress: Int,
                    fromUser: Boolean
                ) {
                    updateHSVtv()
                }

                override fun onStopTrackingTouch(seekBar: SeekBar?) {
                }
            }
        )

        fragmentCameraBinding.control.controlToggleTracking.setOnClickListener {
            if (tracking){
                fragmentCameraBinding.control.controlGameSelector.isEnabled = true
                registeredGames[currentGame].onStop()

                fragmentCameraBinding.control.controlToggleTracking.setImageDrawable(ResourcesCompat.getDrawable( resources , R.drawable.ic_baseline_play_arrow_24 , context?.theme))
                fragmentCameraBinding.control.controlTogglePreview.isEnabled = true
                fragmentCameraBinding.control.controlTogglePreview.alpha = 1.0f

            }else{

                fragmentCameraBinding.control.controlGameSelector.isEnabled = false
                registeredGames[currentGame].onStart()

                fragmentCameraBinding.control.controlToggleTracking.setImageDrawable(ResourcesCompat.getDrawable( resources , R.drawable.ic_baseline_pause_24 , context?.theme))
                fragmentCameraBinding.control.controlTogglePreview.isEnabled = false
                fragmentCameraBinding.control.controlTogglePreview.alpha = 0.5f

            }
            tracking = !tracking
        }

        fragmentCameraBinding.control.controlPreviewNext.setOnClickListener {
            current_preview = (current_preview + 1) % 7;
            updatePreviewName()
        }

        fragmentCameraBinding.control.controlTogglePreview.setOnClickListener {
            if (hsv_clipping){
                fragmentCameraBinding.control.controlTogglePreview.setImageDrawable(ResourcesCompat.getDrawable( resources , R.drawable.ic_baseline_visibility_off_24 , context?.theme))
                fragmentCameraBinding.control.controlToggleTracking.isEnabled = true
                fragmentCameraBinding.control.controlToggleTracking.alpha = 1.0f

                fragmentCameraBinding.control.controlHsvController.visibility = View.INVISIBLE
            }else{
                fragmentCameraBinding.control.controlTogglePreview.setImageDrawable(ResourcesCompat.getDrawable( resources , R.drawable.ic_baseline_visibility_24 , context?.theme))
                fragmentCameraBinding.control.controlToggleTracking.isEnabled = false
                fragmentCameraBinding.control.controlToggleTracking.alpha = 0.5f

                fragmentCameraBinding.control.controlHsvController.visibility = View.VISIBLE
            }
            hsv_clipping = !hsv_clipping
        }

        fragmentCameraBinding.control.controlHsvAuto.setOnClickListener {
            synchronized(dectection_lock){
                synchronized(bitmap_lock){
                    if (detectionResult.size != 1){
                        Toast.makeText(context , "please only detect one object" , Toast.LENGTH_SHORT).show()
                    }else{
                        Toast.makeText(context , "loading..." , Toast.LENGTH_SHORT).show()
                        fragmentCameraBinding.control.controlHsvAuto.isEnabled = false
                        fragmentCameraBinding.control.controlHsvAuto.alpha = 0.5f
                        val rect = detectionResult[0]
                        detectionBuffer = Bitmap.createBitmap(rotatedBuffer)
                        ImageProcessing.Blur(detectionBuffer , detectionBuffer , 15)

                        val w = detectionWidth
                        val h = detectionHeight

                        var pixels = 0
                        Thread {
                            val r = (
                                    min(abs(rect.bottom - rect.top) , abs(rect.left - rect.right))
                                            * min(detectionBuffer.width / w.toDouble(), detectionBuffer.height / h.toDouble())
                                    )

                            val clipFactor = 0.8 // %of central area of the circle

                            val x = (rect.left + rect.right) / 2 * detectionBuffer.width / w.toDouble()
                            val y = (rect.top + rect.bottom) / 2 * detectionBuffer.height / h.toDouble()
                            var xx = 0
                            var yy: Int
                            val hsvMin = Scalar(1000.0, 1000.0,1000.0)
                            val hsvMax = Scalar(0.0, 0.0,0.0)
                            val fArr = FloatArray(3)

                            while (xx < r){
                                yy = 0
                                while (yy < r){
                                    if (((xx - r/2) * (xx - r/2) < r * r / 4) && ((yy - r/2) * (yy - r/2) < r * r / 4 * clipFactor * clipFactor) ){
                                        val rx = xx - r/2 + x
                                        val ry = yy - r/2 + y

                                        val c = detectionBuffer[rx.toInt(), ry.toInt()]
                                        Color.colorToHSV(c , fArr)
                                        fArr[0] *= 255.0f / 360.0f
                                        fArr[1] *= 255.0f
                                        fArr[2] *= 255.0f

                                        hsvMin.`val`[0] = min(hsvMin.`val`[0] , fArr[0].toDouble())
                                        hsvMin.`val`[1] = min(hsvMin.`val`[1] , fArr[1].toDouble())
                                        hsvMin.`val`[2] = min(hsvMin.`val`[2] , fArr[2].toDouble())

                                        hsvMax.`val`[0] = max(hsvMax.`val`[0] , fArr[0].toDouble())
                                        hsvMax.`val`[1] = max(hsvMax.`val`[1] , fArr[1].toDouble())
                                        hsvMax.`val`[2] = max(hsvMax.`val`[2] , fArr[2].toDouble())

                                        pixels++
                                    }
                                    yy++
                                }
                                xx++
                            }

                            fragmentCameraBinding.control.controlHsvAuto.post{
                                fragmentCameraBinding.control.controlHsvAuto.isEnabled = true
                                fragmentCameraBinding.control.controlHsvAuto.alpha = 1.0f

                                fragmentCameraBinding.control.controlHsvHFrom.progress = hsvMin.`val`[0].toInt()
                                fragmentCameraBinding.control.controlHsvSFrom.progress = hsvMin.`val`[1].toInt()
                                fragmentCameraBinding.control.controlHsvVFrom.progress = hsvMin.`val`[2].toInt()

                                fragmentCameraBinding.control.controlHsvHTo.progress = hsvMax.`val`[0].toInt()
                                fragmentCameraBinding.control.controlHsvSTo.progress = hsvMax.`val`[1].toInt()
                                fragmentCameraBinding.control.controlHsvVTo.progress = hsvMax.`val`[2].toInt()

                                Toast.makeText(context , "done! $pixels , $r , $x , $y" , Toast.LENGTH_SHORT).show()
                            }
                        }.start();
                    }
                }
            }
        }
    }

    private fun initSerialControls(){
        fragmentCameraBinding.control.controlToggleSerial.setOnClickListener {
            if (fragmentCameraBinding.control.controlSerial.visibility == View.VISIBLE){
                fragmentCameraBinding.control.controlSerial.visibility = View.INVISIBLE;
                fragmentCameraBinding.control.controlTracking.visibility = View.VISIBLE;
                fragmentCameraBinding.control.controlToggleSerial.setImageDrawable(ResourcesCompat.getDrawable( resources , R.drawable.ic_baseline_chat_bubble_outline_24 , context?.theme))
            }else{
                fragmentCameraBinding.control.controlSerial.visibility = View.VISIBLE;
                fragmentCameraBinding.control.controlTracking.visibility = View.INVISIBLE;
                fragmentCameraBinding.control.controlToggleSerial.setImageDrawable(ResourcesCompat.getDrawable( resources , R.drawable.ic_baseline_chat_bubble_24 , context?.theme))
            }
        }



        fragmentCameraBinding.control.controlCommandSubmit.setOnClickListener {
            if (!General.communication.isConnected){
                Toast.makeText(context , "Not connected" , Toast.LENGTH_SHORT).show()
            } else {
                val text = fragmentCameraBinding.control.controlCommandInput.text.toString();
                try {
                    val cmd = Command.fromString(text)
                    printSerial(">> ${cmd.cmd}\n")
                    General.communication.send(cmd)
                    fragmentCameraBinding.control.controlCommandInput.text.clear()
                } catch (e: java.lang.Exception) {
                    e.printStackTrace();
                }
            }
        }

        fragmentCameraBinding.control.controlSerialOutput.movementMethod = ScrollingMovementMethod()
    }

    @SuppressLint("SetTextI18n")
    fun printSerial(text : String){
        fragmentCameraBinding.control.controlSerialOutput.post{
            if (fragmentCameraBinding.control.controlSerialOutput.text.length > 2048 * 2){
                fragmentCameraBinding.control.controlSerialOutput.text = fragmentCameraBinding.control.controlSerialOutput.text.subSequence(fragmentCameraBinding.control.controlSerialOutput.text.length - 2048 , fragmentCameraBinding.control.controlSerialOutput.text.length)
                //clip it to half
                //TODO: this is a vary ugly way to do it ..
            }
            fragmentCameraBinding.control.controlSerialOutput.append(text)
            val scroll = fragmentCameraBinding.control.controlSerialOutput.layout.getLineTop(fragmentCameraBinding.control.controlSerialOutput.lineCount) - fragmentCameraBinding.control.controlSerialOutput.height
            if (scroll > 0)
                fragmentCameraBinding.control.controlSerialOutput.scrollTo(0 , scroll)
        }
    }

    private fun initBottomSheetControls() {
        // When clicked, lower detection score threshold floor
        fragmentCameraBinding.bottomSheetLayout.thresholdMinus.setOnClickListener {
            if (objectDetectorHelper.threshold >= 0.1) {
                objectDetectorHelper.threshold -= 0.1f
                updateControlsUi()
            }
        }

        // When clicked, raise detection score threshold floor
        fragmentCameraBinding.bottomSheetLayout.thresholdPlus.setOnClickListener {
            if (objectDetectorHelper.threshold <= 0.8) {
                objectDetectorHelper.threshold += 0.1f
                updateControlsUi()
            }
        }

        // When clicked, reduce the number of objects that can be detected at a time
        fragmentCameraBinding.bottomSheetLayout.maxResultsMinus.setOnClickListener {
            if (objectDetectorHelper.maxResults > 1) {
                objectDetectorHelper.maxResults--
                updateControlsUi()
            }
        }

        // When clicked, increase the number of objects that can be detected at a time
        fragmentCameraBinding.bottomSheetLayout.maxResultsPlus.setOnClickListener {
            if (objectDetectorHelper.maxResults < 5) {
                objectDetectorHelper.maxResults++
                updateControlsUi()
            }
        }


    }

    // Update the values displayed in the bottom sheet. Reset detector.
    private fun updateControlsUi() {
        fragmentCameraBinding.bottomSheetLayout.maxResultsValue.text =
            objectDetectorHelper.maxResults.toString()
        fragmentCameraBinding.bottomSheetLayout.thresholdValue.text =
            String.format("%.2f", objectDetectorHelper.threshold)

        // Needs to be cleared instead of reinitialized because the GPU
        // delegate needs to be initialized on the thread using it when applicable
        objectDetectorHelper.clearObjectDetector()
        fragmentCameraBinding.overlay.clear()
    }

    // Initialize CameraX, and prepare to bind the camera use cases
    private fun setUpCamera() {
        val cameraProviderFuture = ProcessCameraProvider.getInstance(requireContext())
        cameraProviderFuture.addListener(
            {
                // CameraProvider
                cameraProvider = cameraProviderFuture.get()

                // Build and bind the camera use cases
                bindCameraUseCases()
            },
            ContextCompat.getMainExecutor(requireContext())
        )
    }

    // Declare and bind preview, capture and analysis use cases
    @SuppressLint("UnsafeOptInUsageError")
    private fun bindCameraUseCases() {

        // CameraProvider
        val cameraProvider =
            cameraProvider ?: throw IllegalStateException("Camera initialization failed.")

        // CameraSelector - makes assumption that we're only using the back camera
        val cameraSelector =
            CameraSelector.Builder().requireLensFacing(CameraSelector.LENS_FACING_BACK).build()

        // Preview. Only using the 4:3 ratio because this is the closest to our models
        preview =
            Preview.Builder()
                .setTargetAspectRatio(AspectRatio.RATIO_4_3)
                .setTargetRotation(fragmentCameraBinding.viewFinder.display.rotation)
                .build()

        // ImageAnalysis. Using RGBA 8888 to match how our models work
        imageAnalyzer =
            ImageAnalysis.Builder()
                .setTargetAspectRatio(AspectRatio.RATIO_4_3)
                .setTargetRotation(fragmentCameraBinding.viewFinder.display.rotation)
                .setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST)
                .setOutputImageFormat(OUTPUT_IMAGE_FORMAT_RGBA_8888)
                .build()
                // The analyzer can then be assigned to the instance
                .also {
                    it.setAnalyzer(cameraExecutor) { image ->

                        if (!::bitmapBuffer.isInitialized) {
                            // The image rotation and RGB image buffer are initialized only once
                            // the analyzer has started running
                            bitmapBuffer = Bitmap.createBitmap(
                              image.width,
                              image.height,
                              Bitmap.Config.ARGB_8888
                            )
                        }
                        detectObjects(image)
                    }
                }

        // Must unbind the use-cases before rebinding them
        cameraProvider.unbindAll()

        try {
            // A variable number of use-cases can be passed here -
            // camera provides access to CameraControl & CameraInfo
            camera = cameraProvider.bindToLifecycle(this, cameraSelector, preview, imageAnalyzer)

            // Attach the viewfinder's surface provider to preview use case
            preview?.setSurfaceProvider(fragmentCameraBinding.viewFinder.surfaceProvider)
        } catch (exc: Exception) {
            Log.e(TAG, "Use case binding failed", exc)
        }
    }


    private fun detectObjects(image: ImageProxy) {
        // Copy out RGB bits to the shared bitmap buffer

        val imageRotation = image.imageInfo.rotationDegrees

         synchronized(bitmap_lock) {
            image.use { bitmapBuffer.copyPixelsFromBuffer(image.planes[0].buffer) }
            val matrix = Matrix()
            matrix.postRotate(imageRotation.toFloat())
            rotatedBuffer = Bitmap.createBitmap(bitmapBuffer , 0 , 0 , bitmapBuffer.width , bitmapBuffer.height , matrix , true)

        }

        // Pass Bitmap and rotation to the object detector helper for processing and detection

        if (!tracking) { //AI-detection mode
            objectDetectorHelper.detect(bitmapBuffer, imageRotation)
        }else if (!hsv_clipping){ //game is active
            val inferenceTime = System.currentTimeMillis()

            selectedColorLow.`val`[0] = fragmentCameraBinding.control.controlHsvHFrom.progress.toDouble()
            selectedColorLow.`val`[1] = fragmentCameraBinding.control.controlHsvSFrom.progress.toDouble()
            selectedColorLow.`val`[2] = fragmentCameraBinding.control.controlHsvVFrom.progress.toDouble()

            selectedColorHigh.`val`[0] = fragmentCameraBinding.control.controlHsvHTo.progress.toDouble()
            selectedColorHigh.`val`[1] = fragmentCameraBinding.control.controlHsvSTo.progress.toDouble()
            selectedColorHigh.`val`[2] = fragmentCameraBinding.control.controlHsvVTo.progress.toDouble()

//            val res = ImageProcessing.DetectObjects(rotatedBuffer , 5 ,
//                selectedColorLow ,
//                selectedColorHigh
//            )

            try {
                if (registeredGames[currentGame].isDisplayEnabled){
                    previewBuffer = Bitmap.createBitmap(registeredGames[currentGame].onCameraFeed(rotatedBuffer))
                }else{
                    registeredGames[currentGame].onCameraFeed(rotatedBuffer)
                }
            }catch (e : Exception){
                e.printStackTrace()
                activity?.runOnUiThread{
                    Toast.makeText(context , "Error while sending camera feed , e: " + e.message , Toast.LENGTH_SHORT).show()
                }
            }

            activity?.runOnUiThread {
                fragmentCameraBinding.bottomSheetLayout.inferenceTimeVal.text =
                    String.format("%d ms", System.currentTimeMillis() - inferenceTime)

                // Pass necessary information to OverlayView for drawing on the canvas
                fragmentCameraBinding.overlay.setResults(
                    registeredGames[currentGame].detectionResult,
                    rotatedBuffer.height,
                    rotatedBuffer.width
                )
                // Force a redraw
                fragmentCameraBinding.overlay.invalidate()

//                previewBuffer = Bitmap.createBitmap(rotatedBuffer)
//
//                when (current_preview) {
//                    (0) -> Utils.matToBitmap(ImageProcessing.src, previewBuffer)
//                    (1) -> Utils.matToBitmap(ImageProcessing.blur, previewBuffer)
//                    (2) -> Utils.matToBitmap(ImageProcessing.hsv, previewBuffer)
//                    (3) -> Utils.matToBitmap(ImageProcessing.mask, previewBuffer)
//                    (4) -> Utils.matToBitmap(ImageProcessing.dilate, previewBuffer);
//                    (5) -> Utils.matToBitmap(ImageProcessing.erode, previewBuffer);
//                    (6) -> Utils.matToBitmap(ImageProcessing.blob, previewBuffer);
//                }
                if (registeredGames[currentGame].isDisplayEnabled) {
                    fragmentCameraBinding.controlBlobPreview.visibility = View.VISIBLE
                    fragmentCameraBinding.controlBlobPreview.setImageBitmap(previewBuffer)
                }else{
                    fragmentCameraBinding.controlBlobPreview.visibility = View.INVISIBLE
                }
            }
        }

        if (hsv_clipping){
            val inferenceTime = System.currentTimeMillis()

            hsvBuffer = Bitmap.createBitmap(rotatedBuffer)

            selectedColorLow.`val`[0] = fragmentCameraBinding.control.controlHsvHFrom.progress.toDouble()
            selectedColorLow.`val`[1] = fragmentCameraBinding.control.controlHsvSFrom.progress.toDouble()
            selectedColorLow.`val`[2] = fragmentCameraBinding.control.controlHsvVFrom.progress.toDouble()

            selectedColorHigh.`val`[0] = fragmentCameraBinding.control.controlHsvHTo.progress.toDouble()
            selectedColorHigh.`val`[1] = fragmentCameraBinding.control.controlHsvSTo.progress.toDouble()
            selectedColorHigh.`val`[2] = fragmentCameraBinding.control.controlHsvVTo.progress.toDouble()

            ImageProcessing.hsv_clip(
                rotatedBuffer ,
                selectedColorLow ,
                selectedColorHigh
            )


            activity?.runOnUiThread {
                fragmentCameraBinding.controlBlobPreview.visibility = View.VISIBLE

                fragmentCameraBinding.bottomSheetLayout.inferenceTimeVal.text =
                    String.format("%d ms", System.currentTimeMillis() - inferenceTime)

                Utils.matToBitmap(ImageProcessing.hsv_clip_result , hsvBuffer)
                fragmentCameraBinding.controlBlobPreview.setImageBitmap(hsvBuffer)
            }
        }

    }

    override fun onConfigurationChanged(newConfig: Configuration) {
        super.onConfigurationChanged(newConfig)
        imageAnalyzer?.targetRotation = fragmentCameraBinding.viewFinder.display.rotation
    }

    // Update UI after objects have been detected. Extracts original image height/width
    // to scale and place bounding boxes properly through OverlayView
    override fun onResults(
      results: MutableList<Detection>?,
      inferenceTime: Long,
      imageHeight: Int,
      imageWidth: Int
    ) {
        synchronized(dectection_lock) {
            detectionResult.clear()
            if (results != null) {
                for (v in results) {
                    detectionResult.add(
                        RectF(
                            v.boundingBox.left,
                            v.boundingBox.top,
                            v.boundingBox.right,
                            v.boundingBox.bottom
                        )
                    );
                }
            }

            detectionWidth = imageWidth
            detectionHeight = imageHeight
        }

        activity?.runOnUiThread {
            fragmentCameraBinding.bottomSheetLayout.inferenceTimeVal.text =
                            String.format("%d ms", inferenceTime)

            // Pass necessary information to OverlayView for drawing on the canvas
            fragmentCameraBinding.overlay.setResults(
                detectionResult,
                imageHeight,
                imageWidth
            )
            // Force a redraw
            fragmentCameraBinding.overlay.invalidate()

            if (!hsv_clipping)
                fragmentCameraBinding.controlBlobPreview.setImageBitmap(null);
        }
    }

    override fun onError(error: String) {
        activity?.runOnUiThread {
            Toast.makeText(requireContext(), error, Toast.LENGTH_SHORT).show()
        }
    }

    var selectedColorLow : Scalar = Scalar(0.0,0.0,0.0)
    var selectedColorHigh : Scalar = Scalar(0.0,0.0,0.0)

    public fun getController(): FragmentCameraBinding {
        return fragmentCameraBinding;
    }

    fun setColorLow(value : Scalar){
        activity?.runOnUiThread {
            fragmentCameraBinding.control.controlHsvHFrom.progress = value.`val`[0].toInt();
            fragmentCameraBinding.control.controlHsvSFrom.progress = value.`val`[1].toInt();
            fragmentCameraBinding.control.controlHsvVFrom.progress = value.`val`[2].toInt();
        }
    }

    fun setColorHigh(value : Scalar){
        activity?.runOnUiThread {
            fragmentCameraBinding.control.controlHsvHTo.progress = value.`val`[0].toInt();
            fragmentCameraBinding.control.controlHsvSTo.progress = value.`val`[1].toInt();
            fragmentCameraBinding.control.controlHsvVTo.progress = value.`val`[2].toInt();
        }
    }


}
