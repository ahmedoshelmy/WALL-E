package uni.proj.rv.fragments;

import androidx.annotation.NonNull;
import androidx.navigation.ActionOnlyNavDirections;
import androidx.navigation.NavDirections;
import uni.proj.rv.R;

public class CameraFragmentDirections {
  private CameraFragmentDirections() {
  }

  @NonNull
  public static NavDirections actionCameraToPermissions() {
    return new ActionOnlyNavDirections(R.id.action_camera_to_permissions);
  }
}
