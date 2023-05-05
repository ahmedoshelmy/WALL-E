package uni.proj.rv.fragments;

import androidx.annotation.NonNull;
import androidx.navigation.ActionOnlyNavDirections;
import androidx.navigation.NavDirections;
import uni.proj.rv.R;

public class PermissionsFragmentDirections {
  private PermissionsFragmentDirections() {
  }

  @NonNull
  public static NavDirections actionPermissionsToCamera() {
    return new ActionOnlyNavDirections(R.id.action_permissions_to_camera);
  }
}
