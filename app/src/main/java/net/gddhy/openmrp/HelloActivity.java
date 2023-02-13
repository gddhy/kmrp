package net.gddhy.openmrp;

import android.*;
import android.app.*;
import android.content.*;
import android.content.pm.*;
import android.os.*;
import android.preference.*;
import android.view.*;
import android.widget.*;
import net.yzjlb.sai.kmrp.qin.*;

import net.yzjlb.sai.kmrp.qin.R;

public class HelloActivity extends Activity{
	SharedPreferences sharedPreferences;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_hello);
		sharedPreferences = PreferenceManager.getDefaultSharedPreferences(this);
	}
	
	public void onClickCheckBox(View view){
		CheckBox checkBox = (CheckBox)view;
		SharedPreferences.Editor editor = sharedPreferences.edit();
		editor.putBoolean("虚拟键盘",checkBox.isChecked());
		editor.apply();
	}
	
	public void onOpenMain(View view){
		if(Build.VERSION.SDK_INT>=Build.VERSION_CODES.M){
			if(hasPermission()){
				openMainActivity();
			} else {
				requestPermission();
			}
		} else {
			openMainActivity();
		}
	}
	
	private boolean hasPermission(){
		return PermissionUtil.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED;
	}
	
	@Override
	public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults){
		super.onRequestPermissionsResult(requestCode, permissions, grantResults);
		if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED){
			openMainActivity();
		} else {
			Toast.makeText(this, "缺少必要权限",Toast.LENGTH_LONG).show();
		}
	}
	
	private void requestPermission(){
		PermissionUtil.requestPermissions(this, new String[] { Manifest.permission.WRITE_EXTERNAL_STORAGE }, 10000);
	}
	
	private void openMainActivity(){
		getPackageManager().setComponentEnabledSetting(new ComponentName(this,"person.wangchen11.nativeview.MainActivity"),
													   PackageManager.COMPONENT_ENABLED_STATE_ENABLED,
													   PackageManager.DONT_KILL_APP);
		openMainActivity(this);
		getPackageManager().setComponentEnabledSetting(new ComponentName(this,"net.gddhy.openmrp.HelloActivity"),
													   PackageManager.COMPONENT_ENABLED_STATE_DISABLED,
													   PackageManager.DONT_KILL_APP);  
		finish();
	}
	
	public static void openMainActivity(Context context){
		Intent intent = new Intent(context,person.wangchen11.nativeview.MainActivity.class);
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_DOCUMENT);
        }
        intent.addFlags(Intent.FLAG_ACTIVITY_MULTIPLE_TASK);
		context.startActivity(intent);
	}
}
