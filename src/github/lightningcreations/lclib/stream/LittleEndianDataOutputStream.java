package github.lightningcreations.lclib.stream;

import java.io.DataOutput;
import java.io.FilterOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;

/**
 * Provided for compatibility with LCLib-C++ IOWrapper (DataOutputStream in little endian byte order mode).
 * Bytes written by this class are suitable for being read by LCLib-C++'s DataInputStream in little-endian byte order mode,
 * or by LittleEndianDataInputStream.
 * Neither the writeBytes, nor writeChars method of This class are recommended to be used. writeUTF should be used to write strings
 * @author Connor
 * @see LittleEndianDataInputStream
 */
public class LittleEndianDataOutputStream extends FilterOutputStream implements DataOutput {

	public LittleEndianDataOutputStream(OutputStream out) {
		super(out);
		// TODO Auto-generated constructor stub
	}

	@Override
	public void writeBoolean(boolean v) throws IOException {
		write(v?1:0);
	}

	@Override
	public void writeByte(int v) throws IOException {
		super.write(v);
	}

	@Override
	public void writeShort(int v) throws IOException {
		super.write(v);
		super.write(v>>8);
	}

	@Override
	public void writeChar(int v) throws IOException {
		writeShort(v);
	}

	@Override
	public void writeInt(int v) throws IOException {
		super.write(v);
		super.write(v>>8);
		super.write(v>>16);
		super.write(v>>24);
	}

	@Override
	public void writeLong(long v) throws IOException {
		writeInt((int)v);
		writeInt((int)(v>>32));
	}

	@Override
	public void writeFloat(float v) throws IOException {
		writeInt(Float.floatToRawIntBits(v));
	}

	@Override
	public void writeDouble(double v) throws IOException {
		writeLong(Double.doubleToRawLongBits(v));

	}

	@Override
	public void writeBytes(String s) throws IOException {
		write(s.getBytes());
	}

	@Override
	public void writeChars(String s) throws IOException {
		for(char c:s.toCharArray())
			writeChar(c);
	}

	@Override
	public void writeUTF(String s) throws IOException {
		byte[] b = s.getBytes(StandardCharsets.UTF_8);
		int len = b.length>65536?65536:b.length;
		writeShort(len);
		write(b,0,len);
	}

}
