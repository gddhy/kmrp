package net.gddhy.openmrp;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.os.Process;
import android.support.annotation.IntRange;
import android.support.annotation.NonNull;

public class PermissionUtil {
    /**
     * Requests permissions to be granted to this application. These permissions
     * must be requested in your manifest, they should not be granted to your app,
     * and they should have protection level {@link android.content.pm.PermissionInfo
     * #PROTECTION_DANGEROUS dangerous}, regardless whether they are declared by
     * the platform or a third-party app.
     * <p>
     * Normal permissions {@link android.content.pm.PermissionInfo#PROTECTION_NORMAL}
     * are granted at install time if requested in the manifest. Signature permissions
     * {@link android.content.pm.PermissionInfo#PROTECTION_SIGNATURE} are granted at
     * install time if requested in the manifest and the signature of your app matches
     * the signature of the app declaring the permissions.
     * </p>
     * <p>
     * If your app does not have the requested permissions the user will be presented
     * with UI for accepting them. After the user has accepted or rejected the
     * requested permissions you will receive a callback reporting whether the
     * permissions were granted or not. Your activity has to implement {@link
     * android.support.v4.app.ActivityCompat.OnRequestPermissionsResultCallback}
     * and the results of permission requests will be delivered to its {@link
     * android.support.v4.app.ActivityCompat.OnRequestPermissionsResultCallback#onRequestPermissionsResult(
     * int, String[], int[])} method.
     * </p>
     * <p>
     * Note that requesting a permission does not guarantee it will be granted and
     * your app should be able to run without having this permission.
     * </p>
     * <p>
     * This method may start an activity allowing the user to choose which permissions
     * to grant and which to reject. Hence, you should be prepared that your activity
     * may be paused and resumed. Further, granting some permissions may require
     * a restart of you application. In such a case, the system will recreate the
     * activity stack before delivering the result to your
     * {@link ActivityCompat.OnRequestPermissionsResultCallback#onRequestPermissionsResult(int, String[], int[])}.
     * </p>
     * <p>
     * When checking whether you have a permission you should use {@link
     * #checkSelfPermission(android.content.Context, String)}.
     * </p>
     * <p>
     * Calling this API for permissions already granted to your app would show UI
     * to the user to decided whether the app can still hold these permissions. This
     * can be useful if the way your app uses the data guarded by the permissions
     * changes significantly.
     * </p>
     * <p>
     * You cannot request a permission if your activity sets {@link
     * android.R.attr#noHistory noHistory} to <code>true</code> in the manifest
     * because in this case the activity would not receive result callbacks including
     * {@link ActivityCompat.OnRequestPermissionsResultCallback#onRequestPermissionsResult(int, String[], int[])}.
     * </p>
     * <p>
     * The <a href="http://developer.android.com/samples/RuntimePermissions/index.html">
     * RuntimePermissions</a> sample app demonstrates how to use this method to
     * request permissions at run time.
     * </p>
     *
     * @param activity The target activity.
     * @param permissions The requested permissions. Must me non-null and not empty.
     * @param requestCode Application specific request code to match with a result
     *    reported to {@link ActivityCompat.OnRequestPermissionsResultCallback#onRequestPermissionsResult(int, String[], int[])}.
     *    Should be >= 0.
     *
     * @see ActivityCompat.OnRequestPermissionsResultCallback#onRequestPermissionsResult(int, String[], int[])
     * @see #checkSelfPermission(android.content.Context, String)
     * @see #shouldShowRequestPermissionRationale(android.app.Activity, String)
     */
    public static void requestPermissions(final @NonNull Activity activity,
                                          final @NonNull String[] permissions, final @IntRange(from = 0) int requestCode) {
        if (Build.VERSION.SDK_INT >= 23) {
            if (activity instanceof RequestPermissionsRequestCodeValidator) {
                ((RequestPermissionsRequestCodeValidator) activity)
                        .validateRequestPermissionsRequestCode(requestCode);
            }
            activity.requestPermissions(permissions, requestCode);
        } else if (activity instanceof OnRequestPermissionsResultCallback) {
            Handler handler = new Handler(Looper.getMainLooper());
            handler.post(new Runnable() {
                @Override
                public void run() {
                    final int[] grantResults = new int[permissions.length];

                    PackageManager packageManager = activity.getPackageManager();
                    String packageName = activity.getPackageName();

                    final int permissionCount = permissions.length;
                    for (int i = 0; i < permissionCount; i++) {
                        grantResults[i] = packageManager.checkPermission(
                                permissions[i], packageName);
                    }

                    ((OnRequestPermissionsResultCallback) activity).onRequestPermissionsResult(
                            requestCode, permissions, grantResults);
                }
            });
        }
    }

    /**
     * @hide
     */
    public interface RequestPermissionsRequestCodeValidator {
        void validateRequestPermissionsRequestCode(int requestCode);
    }

    public interface OnRequestPermissionsResultCallback {

        /**
         * Callback for the result from requesting permissions. This method
         * is invoked for every call on {@link #requestPermissions(android.app.Activity,
         * String[], int)}.
         * <p>
         * <strong>Note:</strong> It is possible that the permissions request interaction
         * with the user is interrupted. In this case you will receive empty permissions
         * and results arrays which should be treated as a cancellation.
         * </p>
         *
         * @param requestCode The request code passed in {@link #requestPermissions(
         * android.app.Activity, String[], int)}
         * @param permissions The requested permissions. Never null.
         * @param grantResults The grant results for the corresponding permissions
         *     which is either {@link android.content.pm.PackageManager#PERMISSION_GRANTED}
         *     or {@link android.content.pm.PackageManager#PERMISSION_DENIED}. Never null.
         *
         * @see #requestPermissions(android.app.Activity, String[], int)
         */
        void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
                                        @NonNull int[] grantResults);
    }

    /**
     * Determine whether <em>you</em> have been granted a particular permission.
     *
     * @param permission The name of the permission being checked.
     *
     * @return {@link android.content.pm.PackageManager#PERMISSION_GRANTED} if you have the
     * permission, or {@link android.content.pm.PackageManager#PERMISSION_DENIED} if not.
     *
     * @see android.content.pm.PackageManager#checkPermission(String, String)
     */
    public static int checkSelfPermission(@NonNull Context context, @NonNull String permission) {
        if (permission == null) {
            throw new IllegalArgumentException("permission is null");
        }

        return context.checkPermission(permission, android.os.Process.myPid(), Process.myUid());
    }
}
