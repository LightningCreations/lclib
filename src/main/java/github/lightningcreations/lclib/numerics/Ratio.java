package github.lightningcreations.lclib.numerics;

/**
 * Ratio is an immutable type which represents a Rational Number.
 * It generally has more precision (but a smaller range of values),
 * than a double.
 * The numerator and denominator 
 * All Fractions represented by the Ratio class are exact and irreducable.
 * For example, The ratio created by Ratio.valueOf(3,9) and Ratio.valueOf(1,3) are equal.
 * Both Reciproricals of Infinity and of 0 are valid (+/-n/0 is +/-Infinity, and +/-n/+/-Infinity is 0)
 * NaN, is never a valid parameter for any method accepting a double.
 * @author connor
 */
public final class Ratio extends Number implements Comparable<Ratio> {
	private static long GCD(long a, long b) {
	   if (b==0) return a;
	   return GCD(b,a%b);
	}
	/**
	 * Returns the ratio that represents 
	 * @param n
	 * @param d
	 */
	public Ratio(long n,long d) {
		n *= Math.signum(d);
		d = Math.abs(d);
		if(d==0) {
			num = n;
			denom = d;
		}else {
			long gcd = GCD(Math.abs(n),d);
			num = n/gcd;
			denom = d/gcd;
		}
	}
	
	public Ratio(double n,double d) {
		if(Double.isNaN(n)||Double.isNaN(d))
			throw new IllegalArgumentException("NaN is not a valid parameter for ratio arguments");
		if(Double.isInfinite(n)) {
			n = 1;
			d = 0;
		}else if(Double.isInfinite(d)) {
			n = 0;
			d = 1;
		}
			
		while(Math.floor(n)!=n&&Math.floor(d)!=d) {
			n *= 2;
			d *= 2;
		}
		num = (long)n;
		denom =(long)(d);
		long gcd = GCD(Math.abs(num),denom);
		num /= gcd;
		denom /= gcd;
	}
	private long num;
	private long denom;
	@Override
	public int intValue() {
		return (int)longValue();
	}

	@Override
	public long longValue() {
		if(denom==1)
			return num;
		else
			return (long)doubleValue();
	}

	@Override
	public float floatValue() {
		// TODO Auto-generated method stub
		return (float)doubleValue();
	}

	@Override
	public double doubleValue() {
		// TODO Auto-generated method stub
		return ((double)num)/denom;
	}
	@Override
	public int compareTo(Ratio o) {
		if(denom!=o.denom)
			return Long.compare(o.denom, denom);
		else
			return Long.compare(num, o.num);
	}
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + (int) (denom ^ (denom >>> 32));
		result = prime * result + (int) (num ^ (num >>> 32));
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
		Ratio other = (Ratio) obj;
		if (denom != other.denom)
			return false;
		if (num != other.num)
			return false;
		return true;
	}
	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return Long.toString(num)+"/"+Long.toString(denom);
	}
	public long getNumerator() {
		return num;
	}
	public long getDenominator() {
		return denom;
	}
	
	public static Ratio valueOf(long l) {
		return new Ratio(l,1);
	}
	public static Ratio valueOf(double d) {
		return new Ratio(d,1);
	}
	public static Ratio valueOf(long num,long denom) {
		return new Ratio(num,denom);
	}
	public static Ratio valueOf(double num,double denom) {
		return new Ratio(num,denom);
	}
	
	public Ratio multiply(double d) {
		return valueOf(num*d,denom);
	}
	public Ratio multiply(long l) {
		return valueOf(num*l,denom);
	}
	public Ratio multiply(Ratio r) {
		return valueOf(num*r.num,denom*r.denom);
	}
	
	public Ratio devide(double d) {
		return valueOf(num,denom*d);
	}
	
	public Ratio devide(long l) {
		return valueOf(num,denom*l);
	}
	public Ratio devide(Ratio r) {
		return valueOf(num*r.denom,denom*r.num);
	}
	public Ratio add(Ratio r) {
		return valueOf(num*r.denom+r.num*denom,(r.denom*denom));
	}
	public Ratio add(double d) {
		return add(valueOf(d));
	}
	public Ratio add(long l) {
		return add(valueOf(l));
	}
	public Ratio negate() {
		return valueOf(-num,denom);
	}
	public Ratio sub(Ratio r) {
		return add(r.negate());
	}
	public Ratio sub(double d) {
		return add(-d);
	}
	public Ratio sub(long l) {
		return add(-l);
	}
	
	public Ratio pow(double x) {
		if(x<0)
			return valueOf(denom,num).pow(x);
		return valueOf(Math.pow(num, x),Math.pow(denom, x));
	}
	
	public Ratio pow(Ratio p) {
		return pow(p.doubleValue());
	}

}
