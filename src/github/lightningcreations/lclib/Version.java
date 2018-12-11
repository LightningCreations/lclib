package github.lightningcreations.lclib;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.Externalizable;
import java.io.IOException;
import java.io.ObjectInput;
import java.io.ObjectOutput;

import github.lightningcreations.lclib.annotation.Constant;
import github.lightningcreations.lclib.annotation.Literal;
import github.lightningcreations.lclib.annotation.NotMutating;
import github.lightningcreations.lclib.annotation.Pattern;

/**
 * The Version class represents a version which can be written to, and read from a stream.
 * This class is provided for consistency with LCLib C++
 */
@Literal
@Pattern("\\d{1,3}\\.\\d{1,3}")
public final class Version implements Comparable<Version>, Externalizable,Cloneable {
	private int major, minor;
	@Constant
	private Version(/*const*/ String[] s) {
		this(Integer.parseInt(s[0]),Integer.parseInt(s[1]));
	}
	/**
	 * Constructs the default Version 1.0
	 */
	@Constant
	public Version() {
		this(1,0);
	}
	/**
	 * Constructs a version from the components (a major and a minor)
	 * @param major The major component of the version (must be between 1 and 256)
	 * @param minor The minor component of the version (must be between 0 and 255)
	 */
	@Constant
	public Version(int major,int minor) {
		this.major = major;
		this.minor = minor;
		if(major>256||major<1||minor>255||minor<0)
			throw new IllegalArgumentException("Version must be between 1.0 and 256.255 inclusive");
	}
	/**
	 * Constructs a version from its encoded form, which is encoded in the Sentry Version format
	 */
	@Constant
	public Version(int encoded) {
		this((encoded>>8)&0xff+1,encoded&0xff);
	}
	/**
	 * Constructs a version from its Text based form (&lt;major&gt;.&lt;minor&gt;)
	 * The represented version must be between 1.0 and 256.255
	 */
	@Constant
	public Version(/*const*/ String s) {
		this(s.split("\\."));
	}
	
	/**
	 * Gets the major component of this version<br/>
	 * This method does not mutate the underlying object (const-qualified)
	 */
	@NotMutating
	public int getMajor() /*const*/ {
		return major;
	}
	
	/**
	 * Gets the minor component of this version<br/>
	 * This method does not mutate the underlying object (const-qualified)
	 */
	@NotMutating
	public int getMinor()/*const*/{
		return minor;
	}
	
	/**
	 * Gets the origin of this version.
	 * The origin of a version is the version with the same major component with 0 in its minor component.<br/>
	 * This method does not mutate the underlying object (const-qualified)
	 */
	@NotMutating
	public /*const*/ Version getOrigin() /*const*/ {
		return new Version(major,0);
	}
	
	/**
	 * Lexicographically Compares a Version to another
	 */
	@Override
	@NotMutating
	public int compareTo(/*const*/Version o)/*const*/ {
		if(major<o.major)
			return -1;
		else if(major>o.major)
			return 1;
		else if(minor<o.minor)
			return -1;
		else if(minor>o.minor)
			return 1;
		else
			return 0;
	}
	/**
	 * Computes the hashCode of a Version. The hashcode of a version is its major*31+its minor<br/>
	 * This method does not mutate the underlying object (const-qualified)<br/>
	 * This method is consistent with equals<br/>
	 * This method is defined as the same as it is in lclib-c++ for consistancy
	 */
	@NotMutating
	@Override
	public int hashCode()/*const*/ {
		return major*31+minor;
	}
	/**
	 * Compares this with annother object for equality.
	 * If the other object is a Version, then they are equal if both the major and minor fields are equal<br/>
	 * This method does not mutate the underlying object (const-qualified)<br/>
	 * This method is consistant with hashCode() and compareTo(Version)
	 * @see java.lang.Object#equals(java.lang.Object)
	 * @param obj The object being compared with. This object is not modified (const-qualified)
	 * @return true if and only if the objects are the same
	 * @see Version#hashCode()
	 * @see Version#compareTo(Version)
	 */
	@NotMutating
	@Override
	public boolean equals(/*const*/ Object obj)/*const*/ {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (Version.class != obj.getClass())
			return false;
		Version other = (Version) obj;
		if (major != other.major)
			return false;
		if (minor != other.minor)
			return false;
		return true;
	}
	
	/**
	 * Converts the version to its string representation
	 * This is &lt;major&gt;.&ltminor&gt;<br/>
	 * This method will not mutate the underlying object (const-qualified)
	 */
	@NotMutating
	@Override
	public /*const*/ String toString()/*const*/{
		return major+"."+minor;
	}
	
	/**
	 * Writes this version (in encoded form)
	 * The encoded form of a Version is (major-1)<<8|minor written in 2 bytes.<br/>
	 * Note: this method is not consistant with the persistance interface of Version class defined in lclib-c++
	 * If consistancy is necessary, the Version should be written and read with write(DataOutput) and read(DataInput) respective
	 * @see write()
	 */
	@Override
	public void writeExternal(ObjectOutput out) /*const*/ throws IOException {
		out.writeByte(major-1);
		out.writeByte(minor);
	}
	/**
	 * Reads a version (from encoded form).
	 * The encoded form of a Version is (major-1)<<8|minor written in 2 bytes.<br/>
	 * Note: this method is not consistant with the persistance interface of Version class defined in lclib-c++
	 * If consistancy is necessary, the Version should be written and read with write(DataOutput) and read(DataInput) respective
	 * @see read(Version)
	 */
	@Override
	public void readExternal(ObjectInput in) throws IOException {
		major = in.readUnsignedByte()+1;
		minor = in.readUnsignedByte();
	}
	/**
	 * Writes this version (in encoded form)
	 * The encoded form of a Version is (major-1)<<8|minor written in 2 bytes.
	 */
	public void write(DataOutput out)/*const*/ throws IOException{
		out.writeByte(major-1);
		out.writeByte(minor);
	}
	/**
	 * Reads a version (from encoded form).
	 * The encoded form of a Version is (major-1)<<8|minor written in 2 bytes.
	 */
	public static Version read(DataInput in)throws IOException{
		return new Version(in.readUnsignedShort());
	}
	public /*const*/ VersionRange sameOrgin()/*const*/{
		return new VersionRange(this,new Version(major,255));
	}
	public VersionRange prior() {
		return new VersionRange(getOrigin(),this);
	}
	public Version clone()/*const*/ {
		return new Version(major,minor);
	}
	public int getEncoded() {
		// TODO Auto-generated method stub
		return (major-1)<<8|minor;
	}
}
