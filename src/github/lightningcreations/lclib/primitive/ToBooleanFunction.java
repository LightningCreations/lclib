package github.lightningcreations.lclib.primitive;

import java.util.function.Predicate;

public interface ToBooleanFunction<T> {
	boolean apply(T val);
	static <T> ToBooleanFunction<T> fromPredicate(Predicate<T> p){
		return p::test;
	}
}
