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
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.features2d.SimpleBlobDetector;
import org.opencv.features2d.SimpleBlobDetector_Params;
import org.opencv.imgproc.Imgproc;

import java.util.LinkedList;
import java.util.List;

public class ImageProcessing {
    private static final String TAG = "ImageProcessor";

    public static Mat src, blur , hsv , mask , dilate , erode , blob;
    public static Mat t0 , t1 , t2;
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

            t0 = new Mat();
            t1 = new Mat();
            t2 = new Mat();

            hsv_clip_result = new Mat();


            Log.i(TAG, "init: OPEN CV LOADED");
            return;
        }
        Log.e(TAG, "init: FAILED TO LOAD OPEN CV");
    }

    public static LinkedList<RectF> DetectColoredBalls(Bitmap map , int blur , Scalar hsv_min , Scalar hsv_max){
        Utils.bitmapToMat(map, src);
        Imgproc.blur(src , ImageProcessing.blur , new Size(blur , blur));
        Imgproc.cvtColor(ImageProcessing.blur , hsv , Imgproc.COLOR_RGB2HSV);
        Core.inRange(hsv , hsv_min, hsv_max , mask);
        Imgproc.dilate(mask , dilate , Imgproc.getStructuringElement(Imgproc.CV_SHAPE_RECT , new Size(3 , 3)));
        Imgproc.erode(dilate , erode , Imgproc.getStructuringElement(Imgproc.CV_SHAPE_RECT , new Size(3 , 3)));


        SimpleBlobDetector_Params params = new SimpleBlobDetector_Params();

        params.set_minThreshold(0);
        params.set_maxThreshold(100);

        params.set_filterByArea(true);

        params.set_minArea(200);
        params.set_maxArea(200000);

        params.set_filterByCircularity(true);
        params.set_minCircularity(0.4f); //0.4 , 0.5 , 0.2 -> best results for green

        params.set_filterByConvexity(true);
        params.set_minConvexity(0.5f);

        params.set_filterByInertia(true);
        params.set_minInertiaRatio(0.2f);

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

    public static LinkedList<RectF> DetectColoredBalls(Bitmap map , int blur , Scalar hsv_min , Scalar hsv_max , int min_area , int max_area){
        Utils.bitmapToMat(map, src);
        Imgproc.blur(src , ImageProcessing.blur , new Size(blur , blur));
        Imgproc.cvtColor(ImageProcessing.blur , hsv , Imgproc.COLOR_RGB2HSV);
        Core.inRange(hsv , hsv_min, hsv_max , mask);
        Imgproc.dilate(mask , dilate , Imgproc.getStructuringElement(Imgproc.CV_SHAPE_RECT , new Size(3 , 3)));
        Imgproc.erode(dilate , erode , Imgproc.getStructuringElement(Imgproc.CV_SHAPE_RECT , new Size(3 , 3)));


        SimpleBlobDetector_Params params = new SimpleBlobDetector_Params();

        params.set_minThreshold(0);
        params.set_maxThreshold(100);

        params.set_filterByArea(true);

        params.set_minArea(min_area);
        params.set_maxArea(max_area);

        params.set_filterByCircularity(true);
        params.set_minCircularity(0.4f); //0.4 , 0.5 , 0.2 -> best results for green

        params.set_filterByConvexity(true);
        params.set_minConvexity(0.5f);

        params.set_filterByInertia(true);
        params.set_minInertiaRatio(0.2f);

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

    public static Point DetectLaser(Bitmap map){
        Utils.bitmapToMat(map, src);
        Imgproc.blur(src , ImageProcessing.blur , new Size(1 , 1));
        Imgproc.cvtColor(ImageProcessing.blur , hsv , Imgproc.COLOR_RGB2HSV);
        Core.inRange(hsv , new Scalar(123,0,255) , new Scalar(195 ,255 ,255) , mask);
        Imgproc.dilate(mask , dilate , Imgproc.getStructuringElement(Imgproc.CV_SHAPE_RECT , new Size(3 , 3)));
        Imgproc.erode(dilate , erode , Imgproc.getStructuringElement(Imgproc.CV_SHAPE_RECT , new Size(3 , 3)));

        Core.MinMaxLocResult result = Core.minMaxLoc(mask);
        return result.maxLoc;
    }



    public static void hsv_clip(Bitmap map , Scalar hsv_min , Scalar hsv_max){
        Utils.bitmapToMat(map, src);
        Imgproc.cvtColor(src , hsv , Imgproc.COLOR_RGB2HSV);
        Core.inRange(hsv , hsv_min, hsv_max , mask);
        //Imgproc.cvtColor(mask , mask , Imgproc.COLOR_GRAY2RGB);
        //Core.bitwise_and(src , mask , hsv_clip_result);

        if (hsv_clip_result.width() != src.width() || hsv_clip_result.height() != src.height()){
            hsv_clip_result = new Mat(src.width() , src.height() , src.type());
        }

        hsv_clip_result.setTo(new Scalar(0,0,0,255));

        Core.copyTo(src , hsv_clip_result , mask);

        //Imgproc.cvtColor(mask , hsv_clip_result , Imgproc.COLOR_HSV2RGB);
    }

    public static void Blur(Bitmap in , Bitmap out ,int blur){
        Utils.bitmapToMat(in, t0);
        Imgproc.blur(t0 , t0 , new Size(blur , blur));
        Utils.matToBitmap(t0 , out);
    }

    //DO NOT USE IT , TOO DAME SLOW
    public static LinkedList<RectF> HoughCircles(Bitmap map , int blur){
        Utils.bitmapToMat(map, src);
        Imgproc.blur(src , ImageProcessing.blur , new Size(blur , blur));
        Imgproc.cvtColor(ImageProcessing.blur , hsv , Imgproc.COLOR_RGB2GRAY);
        Imgproc.HoughCircles(hsv , t1 , Imgproc.HOUGH_GRADIENT ,1 , 100 , 1.0 , 0.9 , 14 , 1000);
        LinkedList<RectF> ret = new LinkedList<>();
        for (int i = 0;i < t1.size().height;i++){
            float[] v = new float[3];
            t1.get(i , 0 , v);
            ret.add(new RectF(v[0] - v[2] , v[1] - v[2] , v[0] + v[2] , v[1] + v[2]));
        }

        return ret;
    }
}
