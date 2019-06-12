package github.lightningcreations.lclib.primitive;

import java.util.function.BiFunction;

public interface BooleanBiFunction<R> extends BiFunction<Boolean, Boolean, R> {
	R apply(boolean a,boolean b);
	default R apply(Boolean a,Boolean b) {
		return apply(!!a,!!b);
	}
}
