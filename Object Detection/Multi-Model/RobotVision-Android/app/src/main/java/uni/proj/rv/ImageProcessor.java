package uni.proj.rv;

import android.graphics.Bitmap;
import android.graphics.RectF;
import android.util.Log;

import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.Core;
import org.opencv.core.KeyPoint;
import org.opencv.core.Mat;
import org.opencv.core.MatOfKeyPoint;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.features2d.SimpleBlobDetector;
import org.opencv.features2d.SimpleBlobDetector_Params;
import org.opencv.imgproc.Imgproc;
import org.opencv.utils.Converters;

import java.util.LinkedList;
import java.util.List;

public class ImageProcessor {
    private static final String TAG = "ImageProcessor";

    public static Mat src, blur , hsv , mask , dilate , erode , blob;
    public static Mat hsv_clip_result;

    public static void init(){
        if (OpenCVLoader.initDebug()) {
            src = new Mat();
            blur = new Mat();
            hsv = new Mat();
            mask = new Mat();
            dilate = new Mat();
            erode = new Mat();
            blob = new Mat();

            Log.i(TAG, "init: OPEN CV LOADED");
            return;
        }
        Log.e(TAG, "init: FAILED TO LOAD OPEN CV");
    }

    public static LinkedList<RectF> ProcessImage(Bitmap map , int blur , Scalar hsv_min , Scalar hsv_max){
        Utils.bitmapToMat(map, src);
        Imgproc.blur(src , ImageProcessor.blur , new Size(blur , blur));
        Imgproc.cvtColor(ImageProcessor.blur , hsv , Imgproc.COLOR_RGB2HSV);
        Core.inRange(hsv , hsv_min, hsv_max , mask);
        Imgproc.dilate(mask , dilate , Imgproc.getStructuringElement(Imgproc.CV_SHAPE_RECT , new Size(3 , 3)));
        Imgproc.erode(dilate , erode , Imgproc.getStructuringElement(Imgproc.CV_SHAPE_RECT , new Size(3 , 3)));


        SimpleBlobDetector_Params params = new SimpleBlobDetector_Params();

        params.set_minThreshold(0);
        params.set_maxThreshold(100);

        params.set_filterByArea(true);

        params.set_minArea(200);
        params.set_maxArea(20000);

        params.set_filterByCircularity(true);
        params.set_minCircularity(0.4f);

        params.set_filterByConvexity(true);
        params.set_minConvexity(0.3f);

        params.set_filterByInertia(true);
        params.set_minInertiaRatio(0.3f);

        SimpleBlobDetector detector = SimpleBlobDetector.create();
        detector.setParams(params);

        Core.bitwise_not(erode , blob);

        MatOfKeyPoint keys = new MatOfKeyPoint();
        detector.detect(blob , keys);
        List<KeyPoint> kl = keys.toList();
        LinkedList<RectF> ret = new LinkedList<>();
        for (int i = 0;i < kl.size();i++){
            RectF r = new RectF();
            KeyPoint k = kl.get(i);
            r.left = (float) k.pt.x - (float) k.size / 2;
            r.top  = (float) k.pt.y - (float) k.size / 2;
            r.right = r.left + (float) k.size ;
            r.bottom = r.top + (float) k.size ;
            ret.add(r);
        }

        return ret;
    }

    public static void hsv_clip(Bitmap map , Scalar hsv_min , Scalar hsv_max){
        Utils.bitmapToMat(map, src);
        Imgproc.cvtColor(src , hsv , Imgproc.COLOR_RGB2HSV);
        Core.inRange(hsv , hsv_min, hsv_max , mask);
        //Imgproc.cvtColor(mask , mask , Imgproc.COLOR_GRAY2RGB);
        //Core.bitwise_and(src , mask , hsv_clip_result);

        if (hsv_clip_result != null)
            hsv_clip_result.release();

        hsv_clip_result = new Mat(src.size() , src.type());
        hsv_clip_result.setTo(new Scalar(0 , 0 , 0 , 255f));
        Core.copyTo(src , hsv_clip_result , mask);
        //Imgproc.cvtColor(mask , hsv_clip_result , Imgproc.COLOR_HSV2RGB);
    }
}
