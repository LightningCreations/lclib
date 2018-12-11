package github.lightningcreations.lclib;

public final class VersionRange {
	private Version start;
	private Version end;
	public static final VersionRange ALL = new VersionRange(new Version(1,0),new Version(256,255));
	public VersionRange(/*const*/ Version start,/*const*/ Version end) {
		this.start = start;
		this.end = end;
	}
	public static /*const*/ VersionRange fromString(/*const*/ String str) {
		if(str.equals("*"))
			return ALL;
		String[] split = str.split("-");
		return new VersionRange(new Version(split[0]),new Version(split[1]));
	}
	public boolean isWithin(/*const */Version v)/*const*/ {
		return start.compareTo(v)<=0&&end.compareTo(v)>0;
	}
	public /*const*/ Version getStart()/*const*/{
		return start;
	}
	public /*const*/ Version getEnd()/*const*/{
		return end;
	}
	public /*const*/ String toString()/*const*/{
		return start+"-"+end;
	}
	

}
