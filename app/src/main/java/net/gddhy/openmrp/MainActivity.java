package net.gddhy.openmrp;

import android.app.*;
import android.content.*;
import android.content.pm.*;
import android.net.*;
import android.os.*;
import android.widget.*;
import com.mrpoid.utils.*;
import java.io.*;
import android.view.KeyEvent;

public class MainActivity extends Activity {
	
    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
		Intent intent = getIntent();
		Uri dat = intent.getData();
		if (dat == null) {
			showError("参数非法！");
			return;
		}
		try{
			InputStream in;
			if(dat.getSchemeSpecificPart().startsWith("file")){
				String path = dat.getPath();
				in = new FileInputStream(path);
			} else {
				in = getContentResolver().openInputStream(dat);
			}
			String path = streamToDSM(in);
			MrpUtils.MrpInfo readMrpInfo = MrpUtils.readMrpInfo(path);
			if(!readMrpInfo.suc){
				showError("当前文件不是mrp");
			} else {//通过包名启动应用
				//PackageManager packageManager = getPackageManager();
				//Intent main = new Intent();
				//main = packageManager.getLaunchIntentForPackage(getPackageName());
				//startActivity(Intent.createChooser(main,"mrp模拟器"));
				person.wangchen11.nativeview.MainActivity mrpMain = ActivityCollector.getActivity(person.wangchen11.nativeview.MainActivity.class);
				if(mrpMain!=null){
					mrpMain.finish();
				}
				
				HelloActivity.openMainActivity(this);
				finish();
			}
		}catch (IOException e){
			showError("文件获取失败");
		}
    }
	
	public static File getDSM_GM_File(){
		return new File(Environment.getExternalStorageDirectory(),"mythroad"+File.separator+"dsm_gm.mrp");
	}

	public static String streamToDSM(InputStream inputStream) throws IOException{
		File file = getDSM_GM_File();
		if (file.exists()){
        	file.delete();
        }
		file.createNewFile();
        OutputStream os = new FileOutputStream(file);
        int read = 0;
        byte[] bytes = new byte[1024 * 1024];
        while ((read = inputStream.read(bytes)) > 0){
            byte[] wBytes = new byte[read];
            System.arraycopy(bytes, 0, wBytes, 0, read);
            os.write(wBytes);
        }
        os.flush();
        os.close();
        inputStream.close();
		return file.getPath();
	}
	
	private void showError(String msg) {
		Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
		finish();
	}
	
	
	
}
