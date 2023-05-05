package uni.proj.rv.fragments;

import java.lang.System;

@kotlin.Metadata(mv = {1, 7, 1}, k = 1, d1 = {"\u0000\u00c4\u0001\n\u0002\u0018\u0002\n\u0002\u0018\u0002\n\u0002\u0018\u0002\n\u0002\b\u0002\n\u0002\u0010\u000e\n\u0000\n\u0002\u0018\u0002\n\u0000\n\u0002\u0018\u0002\n\u0002\b\u0005\n\u0002\u0010\u0000\n\u0000\n\u0002\u0018\u0002\n\u0000\n\u0002\u0018\u0002\n\u0000\n\u0002\u0018\u0002\n\u0000\n\u0002\u0010\b\n\u0002\b\u000b\n\u0002\u0018\u0002\n\u0002\u0018\u0002\n\u0002\b\b\n\u0002\u0010\u000b\n\u0000\n\u0002\u0018\u0002\n\u0000\n\u0002\u0018\u0002\n\u0000\n\u0002\u0018\u0002\n\u0002\b\u0004\n\u0002\u0018\u0002\n\u0002\u0018\u0002\n\u0002\u0018\u0002\n\u0002\b\u0004\n\u0002\u0018\u0002\n\u0002\b\t\n\u0002\u0010\u0002\n\u0002\b\u0002\n\u0002\u0018\u0002\n\u0002\b\u0007\n\u0002\u0018\u0002\n\u0000\n\u0002\u0018\u0002\n\u0000\n\u0002\u0018\u0002\n\u0000\n\u0002\u0018\u0002\n\u0000\n\u0002\u0018\u0002\n\u0002\b\u0005\n\u0002\u0010!\n\u0002\u0018\u0002\n\u0000\n\u0002\u0010\t\n\u0002\b\u000f\u0018\u00002\u00020\u00012\u00020\u0002B\u0005\u00a2\u0006\u0002\u0010\u0003J\b\u0010H\u001a\u00020IH\u0003J\u0010\u0010J\u001a\u00020I2\u0006\u0010K\u001a\u00020LH\u0002J\u0006\u0010M\u001a\u00020\u0007J\b\u0010N\u001a\u00020IH\u0002J\b\u0010O\u001a\u00020IH\u0002J\b\u0010P\u001a\u00020IH\u0002J\b\u0010Q\u001a\u00020IH\u0002J\u0010\u0010R\u001a\u00020I2\u0006\u0010S\u001a\u00020TH\u0016J$\u0010U\u001a\u00020V2\u0006\u0010W\u001a\u00020X2\b\u0010Y\u001a\u0004\u0018\u00010Z2\b\u0010[\u001a\u0004\u0018\u00010\\H\u0016J\b\u0010]\u001a\u00020IH\u0016J\u0010\u0010^\u001a\u00020I2\u0006\u0010_\u001a\u00020\u0005H\u0016J0\u0010`\u001a\u00020I2\u000e\u0010a\u001a\n\u0012\u0004\u0012\u00020c\u0018\u00010b2\u0006\u0010d\u001a\u00020e2\u0006\u0010f\u001a\u00020\u00172\u0006\u0010g\u001a\u00020\u0017H\u0016J\b\u0010h\u001a\u00020IH\u0016J\u001a\u0010i\u001a\u00020I2\u0006\u0010j\u001a\u00020V2\b\u0010[\u001a\u0004\u0018\u00010\\H\u0017J\u0010\u0010k\u001a\u00020I2\u0006\u0010l\u001a\u00020\u0005H\u0007J\u000e\u0010m\u001a\u00020I2\u0006\u0010n\u001a\u00020?J\u000e\u0010o\u001a\u00020I2\u0006\u0010n\u001a\u00020?J\b\u0010p\u001a\u00020IH\u0002J\b\u0010q\u001a\u00020IH\u0002J\b\u0010r\u001a\u00020IH\u0002J\b\u0010s\u001a\u00020IH\u0002R\u000e\u0010\u0004\u001a\u00020\u0005X\u0082D\u00a2\u0006\u0002\n\u0000R\u0010\u0010\u0006\u001a\u0004\u0018\u00010\u0007X\u0082\u000e\u00a2\u0006\u0002\n\u0000R\u001a\u0010\b\u001a\u00020\tX\u0086.\u00a2\u0006\u000e\n\u0000\u001a\u0004\b\n\u0010\u000b\"\u0004\b\f\u0010\rR\u000e\u0010\u000e\u001a\u00020\u000fX\u0082\u000e\u00a2\u0006\u0002\n\u0000R\u0010\u0010\u0010\u001a\u0004\u0018\u00010\u0011X\u0082\u000e\u00a2\u0006\u0002\n\u0000R\u000e\u0010\u0012\u001a\u00020\u0013X\u0082.\u00a2\u0006\u0002\n\u0000R\u0010\u0010\u0014\u001a\u0004\u0018\u00010\u0015X\u0082\u000e\u00a2\u0006\u0002\n\u0000R\u000e\u0010\u0016\u001a\u00020\u0017X\u0082\u000e\u00a2\u0006\u0002\n\u0000R\u001a\u0010\u0018\u001a\u00020\u0017X\u0086\u000e\u00a2\u0006\u000e\n\u0000\u001a\u0004\b\u0019\u0010\u001a\"\u0004\b\u001b\u0010\u001cR\u000e\u0010\u001d\u001a\u00020\u000fX\u0082\u000e\u00a2\u0006\u0002\n\u0000R\u001a\u0010\u001e\u001a\u00020\tX\u0086.\u00a2\u0006\u000e\n\u0000\u001a\u0004\b\u001f\u0010\u000b\"\u0004\b \u0010\rR\u000e\u0010!\u001a\u00020\u0017X\u0082\u000e\u00a2\u0006\u0002\n\u0000R\u0014\u0010\"\u001a\b\u0012\u0004\u0012\u00020$0#X\u0082\u000e\u00a2\u0006\u0002\n\u0000R\u000e\u0010%\u001a\u00020\u0017X\u0082\u000e\u00a2\u0006\u0002\n\u0000R\u0014\u0010&\u001a\u00020\u00078BX\u0082\u0004\u00a2\u0006\u0006\u001a\u0004\b\'\u0010(R\u001a\u0010)\u001a\u00020\tX\u0086.\u00a2\u0006\u000e\n\u0000\u001a\u0004\b*\u0010\u000b\"\u0004\b+\u0010\rR\u000e\u0010,\u001a\u00020-X\u0082\u000e\u00a2\u0006\u0002\n\u0000R\u0010\u0010.\u001a\u0004\u0018\u00010/X\u0082\u000e\u00a2\u0006\u0002\n\u0000R\u000e\u00100\u001a\u000201X\u0082.\u00a2\u0006\u0002\n\u0000R\u0010\u00102\u001a\u0004\u0018\u000103X\u0082\u000e\u00a2\u0006\u0002\n\u0000R\u001a\u00104\u001a\u00020\tX\u0086.\u00a2\u0006\u000e\n\u0000\u001a\u0004\b5\u0010\u000b\"\u0004\b6\u0010\rR\u001e\u00107\u001a\u0012\u0012\u0004\u0012\u00020908j\b\u0012\u0004\u0012\u000209`:X\u0082\u000e\u00a2\u0006\u0002\n\u0000R\u001a\u0010;\u001a\u00020\tX\u0086.\u00a2\u0006\u000e\n\u0000\u001a\u0004\b<\u0010\u000b\"\u0004\b=\u0010\rR\u001a\u0010>\u001a\u00020?X\u0086\u000e\u00a2\u0006\u000e\n\u0000\u001a\u0004\b@\u0010A\"\u0004\bB\u0010CR\u001a\u0010D\u001a\u00020?X\u0086\u000e\u00a2\u0006\u000e\n\u0000\u001a\u0004\bE\u0010A\"\u0004\bF\u0010CR\u000e\u0010G\u001a\u00020-X\u0082\u000e\u00a2\u0006\u0002\n\u0000\u00a8\u0006t"}, d2 = {"Luni/proj/rv/fragments/CameraFragment;", "Landroidx/fragment/app/Fragment;", "Luni/proj/rv/ObjectDetectorHelper$DetectorListener;", "()V", "TAG", "", "_fragmentCameraBinding", "Luni/proj/rv/databinding/FragmentCameraBinding;", "bitmapBuffer", "Landroid/graphics/Bitmap;", "getBitmapBuffer", "()Landroid/graphics/Bitmap;", "setBitmapBuffer", "(Landroid/graphics/Bitmap;)V", "bitmap_lock", "", "camera", "Landroidx/camera/core/Camera;", "cameraExecutor", "Ljava/util/concurrent/ExecutorService;", "cameraProvider", "Landroidx/camera/lifecycle/ProcessCameraProvider;", "currentGame", "", "current_preview", "getCurrent_preview", "()I", "setCurrent_preview", "(I)V", "dectection_lock", "detectionBuffer", "getDetectionBuffer", "setDetectionBuffer", "detectionHeight", "detectionResult", "Ljava/util/LinkedList;", "Landroid/graphics/RectF;", "detectionWidth", "fragmentCameraBinding", "getFragmentCameraBinding", "()Luni/proj/rv/databinding/FragmentCameraBinding;", "hsvBuffer", "getHsvBuffer", "setHsvBuffer", "hsv_clipping", "", "imageAnalyzer", "Landroidx/camera/core/ImageAnalysis;", "objectDetectorHelper", "Luni/proj/rv/ObjectDetectorHelper;", "preview", "Landroidx/camera/core/Preview;", "previewBuffer", "getPreviewBuffer", "setPreviewBuffer", "registeredGames", "Ljava/util/ArrayList;", "Luni/proj/rv/RobotGame;", "Lkotlin/collections/ArrayList;", "rotatedBuffer", "getRotatedBuffer", "setRotatedBuffer", "selectedColorHigh", "Lorg/opencv/core/Scalar;", "getSelectedColorHigh", "()Lorg/opencv/core/Scalar;", "setSelectedColorHigh", "(Lorg/opencv/core/Scalar;)V", "selectedColorLow", "getSelectedColorLow", "setSelectedColorLow", "tracking", "bindCameraUseCases", "", "detectObjects", "image", "Landroidx/camera/core/ImageProxy;", "getController", "initBottomSheetControls", "initSerialControls", "initTrackingControls", "loadGames", "onConfigurationChanged", "newConfig", "Landroid/content/res/Configuration;", "onCreateView", "Landroid/view/View;", "inflater", "Landroid/view/LayoutInflater;", "container", "Landroid/view/ViewGroup;", "savedInstanceState", "Landroid/os/Bundle;", "onDestroyView", "onError", "error", "onResults", "results", "", "Lorg/tensorflow/lite/task/vision/detector/Detection;", "inferenceTime", "", "imageHeight", "imageWidth", "onResume", "onViewCreated", "view", "printSerial", "text", "setColorHigh", "value", "setColorLow", "setUpCamera", "updateControlsUi", "updateHSVtv", "updatePreviewName", "app_debug"})
public final class CameraFragment extends androidx.fragment.app.Fragment implements uni.proj.rv.ObjectDetectorHelper.DetectorListener {
    private final java.lang.String TAG = "ObjectDetection";
    private uni.proj.rv.databinding.FragmentCameraBinding _fragmentCameraBinding;
    private uni.proj.rv.ObjectDetectorHelper objectDetectorHelper;
    public android.graphics.Bitmap bitmapBuffer;
    public android.graphics.Bitmap rotatedBuffer;
    public android.graphics.Bitmap hsvBuffer;
    public android.graphics.Bitmap detectionBuffer;
    public android.graphics.Bitmap previewBuffer;
    private androidx.camera.core.Preview preview;
    private androidx.camera.core.ImageAnalysis imageAnalyzer;
    private androidx.camera.core.Camera camera;
    private androidx.camera.lifecycle.ProcessCameraProvider cameraProvider;
    private boolean tracking = false;
    private int current_preview = 0;
    private boolean hsv_clipping = false;
    private java.lang.Object bitmap_lock;
    private java.lang.Object dectection_lock;
    private java.util.LinkedList<android.graphics.RectF> detectionResult;
    private int detectionWidth = 0;
    private int detectionHeight = 0;
    private java.util.ArrayList<uni.proj.rv.RobotGame> registeredGames;
    private int currentGame = 0;
    
    /**
     * Blocking camera operations are performed using this executor
     */
    private java.util.concurrent.ExecutorService cameraExecutor;
    @org.jetbrains.annotations.NotNull
    private org.opencv.core.Scalar selectedColorLow;
    @org.jetbrains.annotations.NotNull
    private org.opencv.core.Scalar selectedColorHigh;
    
    public CameraFragment() {
        super();
    }
    
    private final uni.proj.rv.databinding.FragmentCameraBinding getFragmentCameraBinding() {
        return null;
    }
    
    @org.jetbrains.annotations.NotNull
    public final android.graphics.Bitmap getBitmapBuffer() {
        return null;
    }
    
    public final void setBitmapBuffer(@org.jetbrains.annotations.NotNull
    android.graphics.Bitmap p0) {
    }
    
    @org.jetbrains.annotations.NotNull
    public final android.graphics.Bitmap getRotatedBuffer() {
        return null;
    }
    
    public final void setRotatedBuffer(@org.jetbrains.annotations.NotNull
    android.graphics.Bitmap p0) {
    }
    
    @org.jetbrains.annotations.NotNull
    public final android.graphics.Bitmap getHsvBuffer() {
        return null;
    }
    
    public final void setHsvBuffer(@org.jetbrains.annotations.NotNull
    android.graphics.Bitmap p0) {
    }
    
    @org.jetbrains.annotations.NotNull
    public final android.graphics.Bitmap getDetectionBuffer() {
        return null;
    }
    
    public final void setDetectionBuffer(@org.jetbrains.annotations.NotNull
    android.graphics.Bitmap p0) {
    }
    
    @org.jetbrains.annotations.NotNull
    public final android.graphics.Bitmap getPreviewBuffer() {
        return null;
    }
    
    public final void setPreviewBuffer(@org.jetbrains.annotations.NotNull
    android.graphics.Bitmap p0) {
    }
    
    public final int getCurrent_preview() {
        return 0;
    }
    
    public final void setCurrent_preview(int p0) {
    }
    
    @java.lang.Override
    public void onResume() {
    }
    
    @java.lang.Override
    public void onDestroyView() {
    }
    
    @org.jetbrains.annotations.NotNull
    @java.lang.Override
    public android.view.View onCreateView(@org.jetbrains.annotations.NotNull
    android.view.LayoutInflater inflater, @org.jetbrains.annotations.Nullable
    android.view.ViewGroup container, @org.jetbrains.annotations.Nullable
    android.os.Bundle savedInstanceState) {
        return null;
    }
    
    @android.annotation.SuppressLint(value = {"MissingPermission"})
    @java.lang.Override
    public void onViewCreated(@org.jetbrains.annotations.NotNull
    android.view.View view, @org.jetbrains.annotations.Nullable
    android.os.Bundle savedInstanceState) {
    }
    
    private final void loadGames() {
    }
    
    private final void updatePreviewName() {
    }
    
    private final void updateHSVtv() {
    }
    
    private final void initTrackingControls() {
    }
    
    private final void initSerialControls() {
    }
    
    @android.annotation.SuppressLint(value = {"SetTextI18n"})
    public final void printSerial(@org.jetbrains.annotations.NotNull
    java.lang.String text) {
    }
    
    private final void initBottomSheetControls() {
    }
    
    private final void updateControlsUi() {
    }
    
    private final void setUpCamera() {
    }
    
    @android.annotation.SuppressLint(value = {"UnsafeOptInUsageError"})
    private final void bindCameraUseCases() {
    }
    
    private final void detectObjects(androidx.camera.core.ImageProxy image) {
    }
    
    @java.lang.Override
    public void onConfigurationChanged(@org.jetbrains.annotations.NotNull
    android.content.res.Configuration newConfig) {
    }
    
    @java.lang.Override
    public void onResults(@org.jetbrains.annotations.Nullable
    java.util.List<org.tensorflow.lite.task.vision.detector.Detection> results, long inferenceTime, int imageHeight, int imageWidth) {
    }
    
    @java.lang.Override
    public void onError(@org.jetbrains.annotations.NotNull
    java.lang.String error) {
    }
    
    @org.jetbrains.annotations.NotNull
    public final org.opencv.core.Scalar getSelectedColorLow() {
        return null;
    }
    
    public final void setSelectedColorLow(@org.jetbrains.annotations.NotNull
    org.opencv.core.Scalar p0) {
    }
    
    @org.jetbrains.annotations.NotNull
    public final org.opencv.core.Scalar getSelectedColorHigh() {
        return null;
    }
    
    public final void setSelectedColorHigh(@org.jetbrains.annotations.NotNull
    org.opencv.core.Scalar p0) {
    }
    
    @org.jetbrains.annotations.NotNull
    public final uni.proj.rv.databinding.FragmentCameraBinding getController() {
        return null;
    }
    
    public final void setColorLow(@org.jetbrains.annotations.NotNull
    org.opencv.core.Scalar value) {
    }
    
    public final void setColorHigh(@org.jetbrains.annotations.NotNull
    org.opencv.core.Scalar value) {
    }
}