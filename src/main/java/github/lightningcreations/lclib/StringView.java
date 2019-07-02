package github.lightningcreations.lclib;

public final class StringView implements CharSequence, Comparable<StringView> {

	public StringView(String s) {
		this(s.toCharArray(),0,s.length());
		strRep = s;
	}
	
	private char[] buffer;
	private int base;
	private int length;
	private String strRep;
	
	public StringView(char[] chars) {
		this(chars.clone(),0,chars.length);
	}
	
	public StringView(StringView v) {
		this(v.buffer,v.base,v.length);
		strRep = v.strRep;
	}

	private StringView(char[] buffer, int base, int length) {
		super();
		this.buffer = buffer;
		this.base = base;
		this.length = length;
	}

	@Override
	public int length() {
		// TODO Auto-generated method stub
		return length;
	}

	@Override
	public char charAt(int index) {
		if(index>length)
			throw new IndexOutOfBoundsException("index is passed the end");
		return buffer[index+base];
	}

	@Override
	public StringView subSequence(int start, int end) {
		if(start>length||end>length)
			throw new IllegalArgumentException("Either start or end is passed the end of the StringView");
		int len = end - start;
		if(len<0)
			throw new IllegalArgumentException("end is before start");
		
		return new StringView(buffer,start,len);
	}

	public StringView substring(int start,int end) {
		return subSequence(start,end);
	}
	
	public String toString() {
		if(strRep==null)
			return strRep = new String(toCharArray());
		else
			return strRep;
	}
	
	public StringView concat(StringView other) {
		return new StringView(toString()+other);
	}
	
	public StringView concat(String other) {
		return new StringView(this+other);
	}
	
	public char[] toCharArray() {
		char[] narr = new char[length];
		System.arraycopy(buffer, base, narr, 0, length);
		return narr;
	}

	@Override
	public int compareTo(StringView o) {
		// TODO Auto-generated method stub
		return toString().compareTo(o.toString());
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((strRep == null) ? 0 : strRep.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		StringView other = (StringView) obj;
		if (!toString().equals(other.toString()))
			return false;
		return true;
	}
	
	public boolean equalsIgnoreCase(StringView other) {
		if(other==null)
			return false;
		else
			return toString().equalsIgnoreCase(other.toString());
	}
	
	
	public int compareToIgnoreCase(StringView other) {
		return toString().compareToIgnoreCase(other.toString());
	}

}
