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
import android.graphics.Canvas
import android.graphics.Matrix
import android.graphics.RectF
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.AdapterView
import android.widget.Toast
import androidx.annotation.RequiresApi
import androidx.camera.core.*
import androidx.camera.core.ImageAnalysis.OUTPUT_IMAGE_FORMAT_RGBA_8888
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.core.content.ContextCompat
import androidx.core.content.res.ResourcesCompat
import androidx.fragment.app.Fragment
import androidx.navigation.Navigation
import org.opencv.android.Utils
import org.opencv.core.Scalar
import org.tensorflow.lite.task.vision.detector.Detection
import uni.proj.ec.Command
import uni.proj.ec.SerialCommunication
import uni.proj.rv.General
import uni.proj.rv.ObjectDetectorHelper
import uni.proj.rv.R
import uni.proj.rv.databinding.FragmentCameraBinding
import java.util.*
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors


class CameraFragment : Fragment(), ObjectDetectorHelper.DetectorListener {

    private val TAG = "ObjectDetection"

    private var _fragmentCameraBinding: FragmentCameraBinding? = null

    private val fragmentCameraBinding
        get() = _fragmentCameraBinding!!

    private lateinit var objectDetectorHelper: ObjectDetectorHelper
    private lateinit var bitmapBuffer: Bitmap
    private lateinit var rotatedBuffer: Bitmap
    private lateinit var processedBuffer: Bitmap

    private var preview: Preview? = null
    private var imageAnalyzer: ImageAnalysis? = null
    private var camera: Camera? = null
    private var cameraProvider: ProcessCameraProvider? = null
    private var tracking = false;
    private var current_preview = 0;
    private var hsv_clipping = false

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
            object :
                SerialCommunication.SerialCommunicationListener {
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
                    (context as Activity?)?.runOnUiThread{
                        printSerial(String(data))
                    }
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

    private fun initTrackingControls(){
        fragmentCameraBinding.control.controlToggleTracking.setOnClickListener {
            if (tracking){
                fragmentCameraBinding.control.controlToggleTracking.setImageDrawable(ResourcesCompat.getDrawable( resources , R.drawable.ic_baseline_play_arrow_24 , context?.theme))
                fragmentCameraBinding.control.controlTogglePreview.isEnabled = true
                fragmentCameraBinding.control.controlTogglePreview.alpha = 1.0f

            }else{
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
            }
            else {
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
    }

    @SuppressLint("SetTextI18n")
    private fun printSerial(text : String){
        fragmentCameraBinding.control.controlSerialOutput.text = fragmentCameraBinding.control.controlSerialOutput.text.toString() + text;
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
        image.use { bitmapBuffer.copyPixelsFromBuffer(image.planes[0].buffer) }
        val imageRotation = image.imageInfo.rotationDegrees

        // Pass Bitmap and rotation to the object detector helper for processing and detection

        if (!tracking) {
            objectDetectorHelper.detect(bitmapBuffer, imageRotation)
        }else {

            rotatedBuffer = if (imageRotation == 90){
                val matrix = Matrix()
                matrix.postRotate(90.0f)
                Bitmap.createBitmap(bitmapBuffer , 0 , 0 , bitmapBuffer.width , bitmapBuffer.height , matrix , true)
            }else{
                Bitmap.createBitmap(bitmapBuffer)
            }

            val inferenceTime = System.currentTimeMillis()
            val res = uni.proj.rv.ImageProcessor.ProcessImage(rotatedBuffer , 5 ,
                Scalar(
                    fragmentCameraBinding.control.controlHsvHFrom.progress.toDouble() ,
                    fragmentCameraBinding.control.controlHsvSFrom.progress.toDouble() ,
                    fragmentCameraBinding.control.controlHsvVFrom.progress.toDouble()
                ) ,
                Scalar(
                    fragmentCameraBinding.control.controlHsvHTo.progress.toDouble() ,
                    fragmentCameraBinding.control.controlHsvSTo.progress.toDouble() ,
                    fragmentCameraBinding.control.controlHsvVTo.progress.toDouble()
                )
            )

            activity?.runOnUiThread {
                fragmentCameraBinding.bottomSheetLayout.inferenceTimeVal.text =
                    String.format("%d ms", System.currentTimeMillis() - inferenceTime)

                // Pass necessary information to OverlayView for drawing on the canvas
                fragmentCameraBinding.overlay.setResults(
                    res,
                    rotatedBuffer.height,
                    rotatedBuffer.width
                )
                // Force a redraw
                fragmentCameraBinding.overlay.invalidate()

                when (current_preview) {
                    (0) -> Utils.matToBitmap(uni.proj.rv.ImageProcessor.src, rotatedBuffer)
                    (1) -> Utils.matToBitmap(uni.proj.rv.ImageProcessor.blur, rotatedBuffer)
                    (2) -> Utils.matToBitmap(uni.proj.rv.ImageProcessor.hsv, rotatedBuffer)
                    (3) -> Utils.matToBitmap(uni.proj.rv.ImageProcessor.mask, rotatedBuffer)
                    (4) -> Utils.matToBitmap(uni.proj.rv.ImageProcessor.dilate, rotatedBuffer);
                    (5) -> Utils.matToBitmap(uni.proj.rv.ImageProcessor.erode, rotatedBuffer);
                    (6) -> Utils.matToBitmap(uni.proj.rv.ImageProcessor.blob, rotatedBuffer);
                }

                fragmentCameraBinding.controlBlobPreview.setImageBitmap(rotatedBuffer)
            }
        }

        if (hsv_clipping){
            rotatedBuffer = if (imageRotation == 90){
                val matrix = Matrix()
                matrix.postRotate(90.0f)
                Bitmap.createBitmap(bitmapBuffer , 0 , 0 , bitmapBuffer.width , bitmapBuffer.height , matrix , true)
            }else{
                Bitmap.createBitmap(bitmapBuffer)
            }

            uni.proj.rv.ImageProcessor.hsv_clip(
                rotatedBuffer ,
                Scalar(
                    fragmentCameraBinding.control.controlHsvHFrom.progress.toDouble() ,
                    fragmentCameraBinding.control.controlHsvSFrom.progress.toDouble() ,
                    fragmentCameraBinding.control.controlHsvVFrom.progress.toDouble()
                ) ,
                Scalar(
                    fragmentCameraBinding.control.controlHsvHTo.progress.toDouble() ,
                    fragmentCameraBinding.control.controlHsvSTo.progress.toDouble() ,
                    fragmentCameraBinding.control.controlHsvVTo.progress.toDouble()
                )
            )

            activity?.runOnUiThread {
                Utils.matToBitmap(uni.proj.rv.ImageProcessor.hsv_clip_result , rotatedBuffer)
                fragmentCameraBinding.controlBlobPreview.setImageBitmap(rotatedBuffer)
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
        val res = LinkedList<RectF>()
        if (results != null) {
            for (v in results){
                res.add(RectF(v.boundingBox.left , v.boundingBox.top , v.boundingBox.right , v.boundingBox.bottom));
            }
        }
        activity?.runOnUiThread {
            fragmentCameraBinding.bottomSheetLayout.inferenceTimeVal.text =
                            String.format("%d ms", inferenceTime)

            // Pass necessary information to OverlayView for drawing on the canvas
            fragmentCameraBinding.overlay.setResults(
                res,
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
}
