package com.sap.ward.ai;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.util.ArrayList;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.iflytek.cloud.speech.Setting;
import com.iflytek.cloud.speech.SpeechConstant;
import com.iflytek.cloud.speech.SpeechError;
import com.iflytek.cloud.speech.SpeechEvent;
import com.iflytek.cloud.speech.SpeechSynthesizer;
import com.iflytek.cloud.speech.SpeechUtility;
import com.iflytek.cloud.speech.SynthesizeToUriListener;
import com.sap.ward.ai.mediaplayer.Player;
import com.sap.ward.ai.model.TulingResponse;
import com.sap.ward.ai.voice.DebugLog;

public class ai
{
	private static String TULING_APIKEY = "123c09c5fb3b8263d7671600fb5dea44";// 官网注册后，换成你自己的
	private static final String APPID = "591932d2";
	private TulingResponse reponse;

	public static void main(String[] args) throws IOException
	{
		if (null != args && args.length > 0 && args[0].equals("true")) {
			// 在应用发布版本中，请勿显示日志，详情见此函数说明。
			Setting.setShowLog(true);
		}
		SpeechUtility.createUtility("appid=" + APPID);

		// String question = "电脑用英语怎么说";// 这是上传给云机器人的问题
		// String question = "大连今天天气怎样";
		// String question = "你叫什么名字";
		String question = "本文档是开发讯飞语音程序的用户指南用英语怎么说";
		new ai().askAndSynthesieThenSpeak(question);
	}

	public void askAndSynthesieThenSpeak(String question) throws IOException
	{
		TulingResponse response = this.getTulingResponse(question);
		speechSynthesis(response.getText());
	}

	public TulingResponse getTulingResponse(String question) throws IOException
	{
		String INFO = URLEncoder.encode(question, "utf-8");
		String getURL = "http://www.tuling123.com/openapi/api?key=" + TULING_APIKEY + "&info=" + INFO;
		URL getUrl = new URL(getURL);
		HttpURLConnection connection = (HttpURLConnection) getUrl.openConnection();
		connection.connect();

		// 取得输入流，并使用Reader读取
		BufferedReader reader = new BufferedReader(new InputStreamReader(connection.getInputStream(), "utf-8"));
		String result = "";
		String line = "";
		while ((line = reader.readLine()) != null) {
			result += line;
		}

		reponse = new ObjectMapper().readValue(result, TulingResponse.class);
		reader.close();
		connection.disconnect();// 断开连接
		DebugLog.Log("[Response from TuLing]: " + reponse);
		return reponse;
	}

	public void speechSynthesis(String words)
	{
		String pcmFile = "./tts_test.pcm";
		SpeechSynthesizer speechSynthesizer = SpeechSynthesizer.createSynthesizer();
		// 设置发音人
		speechSynthesizer.setParameter(SpeechConstant.VOICE_NAME, "xiaoyan");

		// 启用合成音频流事件，不需要时，不用设置此参数
		speechSynthesizer.setParameter(SpeechConstant.TTS_BUFFER_EVENT, "1");
		// 设置合成音频保存位置（可自定义保存位置），默认不保存
		speechSynthesizer.synthesizeToUri(words, pcmFile, synthesizeToUriListener);

	}

	/**
	 * 合成监听器
	 */
	SynthesizeToUriListener synthesizeToUriListener = new SynthesizeToUriListener()
	{

		public void onBufferProgress(int progress)
		{
			DebugLog.Log("[Synthesize progressing]: " + progress);
		}

		public void onSynthesizeCompleted(String uri, SpeechError error)
		{
			if (error == null) {
				DebugLog.Log("[Synthesize successfully, Playing......]");
				DebugLog.Log("[Media file path]: " + uri);
				Player.play(new File(uri));
			} else
				DebugLog.Log("[Error]: " + error.getErrorCode());
			waitupLoop();
		}

		@Override
		public void onEvent(int eventType, int arg1, int arg2, int arg3, Object obj1, Object obj2)
		{
			if (SpeechEvent.EVENT_TTS_BUFFER == eventType) {
				// DebugLog.Log("[onEvent]: type=" + eventType + ", arg1=" +
				// arg1 + ", arg2=" + arg2 + ", arg3=" + arg3
				// + ", obj2=" + (String) obj2);
				ArrayList<?> bufs = null;
				if (obj1 instanceof ArrayList<?>) {
					bufs = (ArrayList<?>) obj1;
				} else {
					DebugLog.Log("onEvent error obj1 is not ArrayList !");
				} // end of if-else instance of ArrayList

				if (null != bufs) {
					for (final Object obj : bufs) {
						if (obj instanceof byte[]) {
							final byte[] buf = (byte[]) obj;
							// DebugLog.Log("onEvent buf length: " +
							// buf.length);
						} else {
							DebugLog.Log("onEvent error element is not byte[] !");
						}
					} // end of for
				} // end of if bufs not null
			} // end of if tts buffer event
		}

	};

	private void waitupLoop()
	{
		synchronized (this) {
			ai.this.notify();
		}
	}

}
