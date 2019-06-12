package github.lightningcreations.lclib;

import java.util.Set;
import java.util.HashSet;

public interface Sets {
	public static <E> Set<E> union(Set<? extends E> s1,Set<? extends E> s2){
		Set<E> ret = new HashSet<E>(s1);
		ret.addAll(s2);
		return Set.copyOf(ret);
	}
	public static <E> Set<E> disjunction(Set<? extends E> s1,Set<? extends E> s2){
		Set<E> f = new HashSet<>(s1);
		f.removeAll(s2);
		Set<E> g = new HashSet<>(s2);
		g.removeAll(s1);
		return union(f,g);
	}
	public static <E> Set<E> intersection(Set<? extends E> s1,Set<? extends E> s2){
		Set<E> ret = new HashSet<E>(s1);
		ret.retainAll(s2);
		return Set.copyOf(ret);
	}
}
