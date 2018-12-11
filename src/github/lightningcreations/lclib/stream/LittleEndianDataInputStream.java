package github.lightningcreations.lclib.stream;

import java.io.DataInput;
import java.io.EOFException;
import java.io.FilterInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;

/**
 * Provided for compability with LCLib-C++ IOWrapper DataInputStream (open in little endian mode).
 * Consider all methods to have same documentation as there counterpart in said class.
 * readUTF() is the counterpart method from readString().
 * It is not recommended to use readLine() in this class. (no effort will be made to ensure little-endian byte order is respected).
 * @author Connor
 * @see LittleEndianDataOutputStream
 */
public class LittleEndianDataInputStream extends FilterInputStream implements DataInput {

	public LittleEndianDataInputStream(InputStream in) {
		super(in);
		// TODO Auto-generated constructor stub
	}
	private int readSingle()throws IOException{
		int val = super.read();
		if(val<0)
			throw new EOFException();
		return val;
	}

	@Override
	public void readFully(byte[] b) throws IOException {
		readFully(b,0,b.length);
	}

	@Override
	public void readFully(byte[] b, int off, int len) throws IOException {
		if(super.read(b, off, len)<len)
			throw new EOFException();
	}

	@Override
	public int skipBytes(int n) throws IOException {
		// TODO Auto-generated method stub
		return (int) super.skip(n);
	}

	@Override
	public boolean readBoolean() throws IOException {
		// TODO Auto-generated method stub
		return readByte()!=0;
	}

	@Override
	public byte readByte() throws IOException {
		// TODO Auto-generated method stub
		return (byte)readSingle();
	}

	@Override
	public int readUnsignedByte() throws IOException {
		// TODO Auto-generated method stub
		return readSingle();
	}

	@Override
	public short readShort() throws IOException {
		// TODO Auto-generated method stub
		return (short)(readSingle()|readSingle()<<8);
	}

	@Override
	public int readUnsignedShort() throws IOException {
		// TODO Auto-generated method stub
		return readSingle()|readSingle()<<8;
	}

	@Override
	public char readChar() throws IOException {
		// TODO Auto-generated method stub
		return (char)readUnsignedShort();
	}

	@Override
	public int readInt() throws IOException {
		// TODO Auto-generated method stub
		return readSingle()|readSingle()<<8|readSingle()<<16|readSingle()<<24;
	}

	@Override
	public long readLong() throws IOException {
		// TODO Auto-generated method stub
		return readInt()|(readInt()<<32L);
	}

	@Override
	public float readFloat() throws IOException {
		// TODO Auto-generated method stub
		return Float.intBitsToFloat(readInt());
	}

	@Override
	public double readDouble() throws IOException {
		// TODO Auto-generated method stub
		return Double.longBitsToDouble(readLong());
	}

	@Override
	public String readLine() throws IOException {
		Scanner scan = new Scanner(this);
		return scan.nextLine();
	}

	@Override
	public String readUTF() throws IOException {
		int len = readUnsignedShort();
		byte[] b = new byte[len];
		readFully(b);
		return new String(b,StandardCharsets.UTF_8);
	}

}
