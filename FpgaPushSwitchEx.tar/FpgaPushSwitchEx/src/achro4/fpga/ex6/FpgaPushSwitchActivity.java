package achro4.fpga.ex6;

import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.media.AudioManager;
import android.media.SoundPool;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.EditText;

public class FpgaPushSwitchActivity extends Activity {

	//quiz initializing
			private int[][]quiz=
					{{1, 2, 4},//도레미
					{4, 8, 16},//미파솔
					{1, 4, 16},//도미솔
					{8, 16, 32},//파솔라
					{8, 32, 128},//파라도^
					{4, 1, 16},//미도_솔
					{32, 64, 16},//라시솔
					{32, 2, 64}};//라레시
			private int[][]quiz_sound=
					{{1, 2, 3},//도레미
					{3, 4, 5},//미파솔
					{1, 3, 5},//도미솔
					{4, 5, 6},//파솔라
					{4, 6, 8},//파라도^
					{3, 1, 5},//미도_솔
					{6, 7, 5},//라시솔
					{6, 2, 7}};//라레시

			private TextView stage;
		    private Button start;
		    private Button again;
		    private TimerTask mTimerTask;
		    private Timer mainTimer = new Timer();
		    private int stage_num = 0;
			public int push_count = 0;
		    public int value = 100;
		    public String str;
		    public String time;
		    public TextView button0;
		    public TextView button1;
			private void stopTimer() {
				//꺼뜨리고
				
				mTimerTask.cancel();
			}

			private Handler updateHandler = new Handler() {
				@Override
				public void handleMessage(Message msg) {
					//FpgaSetLedValue(value);
					stage.setText(str);
					if(str.equals("1")) {
						start.setVisibility(View.INVISIBLE);
					}
				}
			};

			    @Override
			    protected void onCreate(Bundle savedInstanceState) {
			        super.onCreate(savedInstanceState);
			        setContentView(R.layout.main);
			        
			        FpgaSetMotorState(0,0,0);
					 FpgaSetLedValue(0);
					 FpgaSetDotValue(1);
			      
			        
			        final SoundPool sp = new SoundPool(1, AudioManager.STREAM_MUSIC, 0);

			        final int c = sp.load(this, R.raw.c, 1); //1
					 final int d = sp.load(this, R.raw.d, 1); //2
			        final int e = sp.load(this, R.raw.e, 1); //4
			        final int f = sp.load(this, R.raw.f, 1); //8
			        final int g = sp.load(this, R.raw.g, 1); //16
			        final int a = sp.load(this, R.raw.a, 1); //32
			        final int b = sp.load(this, R.raw.b, 1); //64
			        final int up_c = sp.load(this, R.raw.up_c, 1); //128
			    
			        stage =  (TextView)findViewById(R.id.stage);
			        start = (Button)findViewById(R.id.start);

			        FpgaSetTextLcdValue("start...", "good luck!");

		            TimerTask task = new TimerTask() {
		            	Handler mHandler = new Handler();
		            	
		            	public void run() {
		            		mHandler.postDelayed(new Runnable() {
		            			
		            			public void run() {

		            			    int Value;	    
		            			    Value = DeviceOpen();
		            			    
		            			    if(Value != -1) {
		            			    	Value = ReceivePushSwitchValue();
									}
		            			    if(Value != -1){
		            			    	DeviceClose();
											if (Value == quiz[stage_num][push_count]) {
												push_count++;
												if (push_count == 3) {
													FpgaSetMotorState(3,10,10);
												
													stage_num++;
													FpgaSetDotValue((stage_num+1));
													stage.setText("Stage " + ( stage_num+1 ));
													push_count=0;
													stopTimer();
													value=100;
													if(stage_num == 8) {
														FpgaSetBuzzerValue(1);
														stage.setText("Congratulations!!!");
														FpgaSetTextLcdValue("chuka", "  chuka");
														start.setVisibility(View.INVISIBLE);
														return;
													}
												}
											} else {
												//틀린것 처리
											
											}

											if (Value != -1) {

												switch (Value) {
													case 1:
														sp.play(c, 1, 1, 0, 0, 1);

														break;
													case 2:
														sp.play(d, 1, 1, 0, 0, 1);
														break;
													case 4:
														sp.play(e, 1, 1, 0, 0, 1);
														break;
													case 8:
														sp.play(f, 1, 1, 0, 0, 1);
														break;
													case 16:
														sp.play(g, 1, 1, 0, 0, 1);
														break;
													case 32:			            	FpgaSetLedValue(127);
														sp.play(a, 1, 1, 0, 0, 1);
														break;
													case 64:
														sp.play(b, 1, 1, 0, 0, 1);
														break;
													case 128:
														sp.play(up_c, 1, 1, 0, 0, 1);
														break;
													default:
														break;
												}
												String str = Integer.toString(Value, 16);
											}
										
									}

		            			}
		            		}, 100);
		            	}            	
		            	
		            };
			        
			        Timer t = new Timer();
			        
			        t.schedule(task, 100, 100);
			        
			        start.setOnClickListener(new View.OnClickListener() {
			            @Override
			            public void onClick(View v) {
			            	FpgaSetLedValue(127);
			            	FpgaSetMotorState(0,0,0);
			            	ReceiveFndValue("7777");
								for (int i = 0; i < 3; i++) {
									sp.play(quiz_sound[stage_num][i], 1, 1, 0, 0, 1);
									try {
										Thread.sleep(500);
									} catch (Exception e) {
										e.printStackTrace();
									}
								}
			                timerMethod();

			            }
			        });
			    }


			        
			    private final void timerMethod() {
			        mTimerTask = new TimerTask() {
			            @Override
			            public void run() {
			                value--;
			                	
			                if(value==0) {
			                    stopTimer();
			                    ReceiveFndValue("0000");
			                    FpgaSetBuzzerValue(1);
			                    FpgaSetTextLcdValue("Sorry.. to", "hear that");
			                  
			                    return;
			                }    
			                str = Integer.toString(value, 10);
			                updateHandler.sendEmptyMessage(0);
			           
			            }
			        };



			       mainTimer.schedule(mTimerTask, 0, 100);
			    }

			    public native int FpgaSetBuzzerValue(int x);
			    public native int FpgaSetTextLcdValue(String ptr1, String ptr2);
			    public native String FpgaSetMotorState(int x, int y, int z);
			    public native void FpgaSetLedValue(int x);
			    public native String FpgaSetDotValue(int x);
			    public native int ReceiveFndValue(String str);
			    public native int ReceivePushSwitchValue();
			    public native int DeviceOpen();
			    public native int DeviceClose();



			    static {
			        System.loadLibrary("fpga-push-switch-jni");
			    }

}