package github.lightningcreations.lclib.primitive;

import java.util.function.BiFunction;

public interface BooleanObjectBiFunction<A, R> extends BiFunction<Boolean, A, R> {
	R apply(boolean a,A b);
	default R apply(Boolean a,A b) {
		return apply(!!a,b);
	}
}
