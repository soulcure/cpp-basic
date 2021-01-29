package proto.youmai.com;

import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class IMContentUtil {
	
	
	private static String LEFTBRACKET = "<" ;
	private static String RIGHTBRACKET = ">" ;
	private static String ENDSLASH = "/" ;
	private String msgBody ;
	/**
	 * start from 0.
	 */
	private int position = 0; 
	private List<IMContentItem> contentList  ;
	
	
	public IMContentUtil() {
		// TODO Auto-generated constructor stub
		contentList = new ArrayList<IMContentItem>() ;
		msgBody = "";
	}
 
	public IMContentUtil(String _msgBody){
		contentList = new ArrayList<IMContentItem>() ;
		msgBody  = _msgBody ;
	}
	
	public void parseBody(String _msgBody) {
		msgBody = _msgBody ;
		parseBody();
	}

	private int getContentType(String _name){
		return IMContentType.valueOf(_name).ordinal();
	}


	public void parseBody(){

		JSONArray array = null;
		try {
			array = new JSONArray(msgBody);
			for (int i = 0; i < array.length(); i++) {
				JSONObject obj = array.getJSONObject(i);

				Iterator it = obj.keys();
				while (it.hasNext()) {
					String key = (String) it.next();
					String value = obj.getString(key);
					IMContentItem item = new IMContentItem();
					item.item = value;
					item.itemType = IMContentType.valueOf(key.toString());
					contentList.add(item);

					Log.d("json", key + " val:" + value);
					System.out.println("System.out.println" + key + " val:" + value);
				}
			}
		}catch (JSONException e) {
			e.printStackTrace();
		}


//		JSONArray newArray = new JSONArray();
//		JSONObject newJson = new JSONObject();
//		try {
//			JSONObject obj = new JSONObject(msgBody);
//			Iterator it = obj.keys();
//			while (it.hasNext()) {
//				String key = (String) it.next();
//				String value = obj.getString(key);
//				IMContentItem item = new IMContentItem();
//				item.item =value ;
//				item.itemType = IMContentType.valueOf(key.toString()) ;
//				contentList.add(item);
//
//				Log.d("json", key + " val:" + value);
//				System.out.println("System.out.println" + key+  " val:" + value);
//			}
//			newJson.put("groups",newArray);
//			System.out.println(newJson);
//		} catch (JSONException e) {
//			System.out.println(e.toString());
//		}
	}



	public String serializeToString(){
		String body ="";
//		JSONObject obj = new JSONObject();
//		try {
//			for (int i = 0; i < contentList.size(); i++) {
//				IMContentItem item = contentList.get(i);
//						obj.put(item.itemType.toString(), item.item);
//			}
//		} catch (JSONException e) {
//			e.printStackTrace();
//		}

		JSONArray arr = new JSONArray();
		try {
			for (int i = 0; i < contentList.size(); i++) {
				IMContentItem item = contentList.get(i);
				JSONObject obj = new JSONObject();
				obj.put(item.itemType.toString(), item.item);
				arr.put(obj);
			}
		} catch (JSONException e) {
			e.printStackTrace();
		}

 		return arr.toString();
	}
	
	private void addItem(IMContentType _type, String _item) {
		IMContentItem item = new IMContentItem();
		item.item = _item ;
		item.itemType = _type ;
		contentList.add(item);
	}
	
	/**
	 * insert some text into IM Body.
	 * @param _text
	 */
	public void appendText(String _text) {
		if (contentList.size()>0) {
			IMContentItem item = contentList.get( contentList.size()- 1) ;
			if (item.itemType==IMContentType.CONTENT_TEXT) {
				item.item += _text ;
				contentList.set(contentList.size()-1, item);
			}else {
				addItem(IMContentType.CONTENT_TEXT,_text);
			}
		}else {
			addItem(IMContentType.CONTENT_TEXT,_text);
		}
	}
	
	public void appendImage(String _imageUrl) {
		addItem( IMContentType.CONTENT_IMAGE, _imageUrl);
	}
	public void appendFile(String _fileUrl) {
		addItem( IMContentType.CONTENT_FILE, _fileUrl);
	}
	
	public void appendUrl(String _url) {
		addItem( IMContentType.CONTENT_URL, _url);
	}
	
	/**
	 * insert at some one.
	 * @param _atBody
	 * 		user_id;organ_id
	 */
	public void appendAT(String _atBody) {
		addItem(IMContentType.CONTENT_AT, _atBody);
	}
	
	/**
	 * set current position 0.
	 */
	public void reset() {
		position = 0;
	}
	
	/**
	 * 
	 * @return
	 * 		if, has unread IMContent, return next contentType.
	 * 		else return null.
	 */
	public IMContentType hasNext() {
		if (contentList.size() > position ) {
			return contentList.get( position ).itemType;
		}
		return null;
	}
	/**
	 * read next element. current position will actual move to next. 
	 * @return
	 * 		if has return element.
	 * 		else return null.
	 */
	public String readNext() {
		
		if (contentList.size() > position ) {
			
			return contentList.get( position++ ).item;
		}
		
		return null;
	}
	
	/**
	 * remove from end. delete one char or rich element.
	 * @return
	 * 		if and only succeed delete one, return true.
	 */
	public boolean remove() {
		if (contentList.isEmpty()) {
			return false;
		}else {
			IMContentItem item = contentList.get( contentList.size() - 1 );
			if (item.itemType != IMContentType.CONTENT_TEXT) {
				contentList.remove( contentList.size() - 1 );
				return true;
			}else {// this is Text info.
				if (item.item.isEmpty()) {
					contentList.remove( contentList.size() - 1 );
					return true;
				}else {
					item.item = item.item.substring(0, item.item.length()-1);
					contentList.remove( contentList.size() - 1 );
					contentList.add(item);
					return true;
				}
			}
		}
	}
	/**
	 * "helllo[Picture],world"
	 * "[Picture]helllo,world"	
	 * @param caretPos
	 * 		start from 0. caret treat rich element as a whole.
	 * @return
	 * 		if, and only remove one char or a element at caretpos, return true;
	 */
	public boolean removeAtIndex(int caretPos) {
		int lookuploop = 0;
		for (int i = 0; i < contentList.size(); i++) {
			IMContentItem item = contentList.get(i);
			if (item.itemType != IMContentType.CONTENT_TEXT) {
				if (lookuploop == caretPos) {
					// got it.
					contentList.remove(i); 
					return true;
				}
				++lookuploop;
			} else { // Text
				if (lookuploop + item.item.length() > caretPos) {
					// it contains.
					String newText = item.item.substring(0, caretPos - lookuploop ) ;
					if ( item.item.length() > caretPos - lookuploop + 1) {
						newText += item.item.substring(caretPos - lookuploop + 1);
					}
							
					item.item = newText;
					contentList.set(i, item);
					return true;
				} else {
					lookuploop += item.item.length();
				}
			}

		}

		return false;
	}

}
