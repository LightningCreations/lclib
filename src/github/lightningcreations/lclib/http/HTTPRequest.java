package github.lightningcreations.lclib.http;

import java.io.IOException;
import java.net.InetAddress;
import java.net.URL;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;
import java.util.TreeMap;
import java.util.function.Function;

import javax.imageio.ImageReader;
import javax.sound.sampled.AudioInputStream;

public class HTTPRequest {
	private boolean useTLS;
	private InetAddress target;
	private String path;
	private String version;
	private Map<String,String> fields;
	private String contentType;
	private ByteBuffer content = ByteBuffer.allocate(0);
	private int port;
	private String method;
	private String query;
	private static String getImageContentType(String fmt) {
		switch(fmt.toUpperCase()) {
		case "JPEG":return "image/jpeg";
		case "PNG": return "image/png";
		case "BMP": return "image/bmp";
		case "GIF": return "image/gif";
		case "TIFF": return "image/tiff";
		}
		return "application/octet-stream";
	}
	private static final Map<Class<?>,String> contentTypeMap;
	private static final Map<Class<?>,Function<Object,ByteBuffer>> encodingMap;
	private static final Map<Class<?>,Function<ByteBuffer,?>> decodingMap;
	public static <T> void registerContentType(Class<T> cl,String contentType,Function<? super T,ByteBuffer> enc,Function<ByteBuffer,? extends T> dec) {
		contentTypeMap.putIfAbsent(cl, contentType);
		encodingMap.putIfAbsent(cl,(Function<Object,ByteBuffer>) enc);
		decodingMap.putIfAbsent(cl, dec);
	}
	/**
	 * Checks if the given class or any of its superclasses has been registered.
	 * Same Complexity as getContentTypeClassKey(cl)
	 * @param cl the class to check
	 * @return true if and only if the class or any supertype has been registered
	 */
	public static boolean hasRegisteredContentType(Class<?> cl) {
		if(cl==Object.class)
			return false;
		else if(contentTypeMap.containsKey(cl))
			return true;
		else if(cl.getSuperclass()!=null&&hasRegisteredContentType(cl.getSuperclass()))
			return true;
		else if(cl.getInterfaces().length!=0)
			for(Class<?> i:cl.getInterfaces())
				if(hasRegisteredContentType(i))
					return true;
		return false;
	}
	/**
	 * Computes the Content Type Key to lookup the mime type.
	 * Best case: cl is either Object, has only Object as its superclass (and no interfaces),
	 * or cl is directly registered: lookup is constant complexity
	 * Other Cases: Average Complexity of O(n^2) given that n is the total number of superclasses (that are not Object),
	 * and interfaces inherited by n and its parents
	 * Provided so that Superclasses can be registered without knowledge of subclasses.
	 * If a class has multiple registered supertypes which are valid entries,
	 * It is unspecified which of these is returned.
	 * @param cl The class to lookup
	 * @return null if the class and none of its supertypes where registered with registerContentType, or the specific class which was registered
	 */
	private static Class<?> getContentTypeClassKey(Class<?> cl){
		if(cl==Object.class)
			return null;
		else if(contentTypeMap.containsKey(cl))
			return cl;
		else if(cl.getSuperclass()!=null&&hasRegisteredContentType(cl.getSuperclass()))
			return getContentTypeClassKey(cl.getSuperclass());
		else for(Class<?> i:cl.getInterfaces())
			if(hasRegisteredContentType(i))
				return getContentTypeClassKey(i);
		return null;
	}
	public static String getRegisteredContentType(Class<?> cl) {
		if(contentTypeMap.containsKey(cl))
			return contentTypeMap.get(cl);
		else 
			return contentTypeMap.get(getContentTypeClassKey(cl));
	}
	static {
		contentTypeMap = new HashMap<>();
		encodingMap = new HashMap<>();
		decodingMap = new HashMap<>();
		registerContentType(String.class,"text/plain;encoding=utf8",s->ByteBuffer.wrap(s.getBytes(StandardCharsets.UTF_8)),b->new String(b.array(),StandardCharsets.UTF_8));
		registerContentType(Number.class,"text/plain;encoding=utf8",i->encode(i.toString()),null);//Will never get decoded
		registerContentType(Character.class,"text/plain;encoding=utf8",c->encode(new char[] {c}),null);//Will never get decoded
		registerContentType(char[].class,"text/plain;encoding=utf8",c->encode(new String(c)),null);
		registerContentType(ByteBuffer.class,"application/octet-stream",b->b,b->b);
		registerContentType(byte[].class,"application/octet-stream",b->ByteBuffer.wrap(b),b->b.array());
	}
	public static <T> ByteBuffer encode(T o) {
		if(o.getClass()==Object.class)
			return null;
		Class<?> cl = getContentTypeClassKey(o.getClass());
		if(cl==null)
			return null;
		else
			return encodingMap.get(cl).apply(o);
	}
	public static String getContentTypeForObject(Object o)throws IOException {
		Class<?> cl = o.getClass();
		if(o instanceof ImageReader) {
			ImageReader img = (ImageReader)o;
			return getImageContentType(img.getFormatName());
		}else if(o instanceof AudioInputStream) {
			return "application/octet-stream";//TODO
		}else if(hasRegisteredContentType(cl))
			return getRegisteredContentType(cl);
		return null;
	}
	public HTTPRequest(String method,URL u) throws UnknownHostException {
		if(u.getProtocol()=="https")
			useTLS = true;
		target = InetAddress.getByName(u.getHost());
		path = u.getPath();
		query = u.getQuery();
		fields = new TreeMap<>(String::compareToIgnoreCase);
		version = "HTTP/1.1";
	}
	public void setField(String name,String field) {
		fields.put(name, field);
	}
	public <T> void setContent(T o) throws IOException {
		if(getContentTypeForObject(o)!=null) {
			contentType = getContentTypeForObject(o);
			content = encode(o);
		}
	}
	public void setContent(ByteBuffer buff,String contentType) {
		content = buff;
		this.contentType = contentType;
	}
	public HTTPResponse makeRequest() {
		fields.putIfAbsent("User-Agent", "LCLibHTTP/1.0");
		fields.put("Transfer-Encoding", "identity");
		fields.put("Content-Type", contentType);
		fields.put("ContentLength", String.valueOf(content.capacity()));
		StringBuilder httpRequestHeader = new StringBuilder()
				.append(method).append(" ").append(path).append(" ").append(version).append("\r\n")
				.append("Host: ").append(target.getHostName()).append("\r\n");
		for(Map.Entry<String, String> entry:fields.entrySet())
			httpRequestHeader.append(entry.getKey()).append(": ").append(entry.getValue()).append("\r\n");
		httpRequestHeader.append("\r\n");
		byte[] buff = httpRequestHeader.toString().getBytes(StandardCharsets.US_ASCII);
		ByteBuffer total = ByteBuffer.allocate(buff.length+content.capacity());
		total.put(buff);
		total.put(content);
		total.position(0);
		return null;
	}
}
