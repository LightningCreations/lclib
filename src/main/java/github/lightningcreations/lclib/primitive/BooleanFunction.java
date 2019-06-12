package github.lightningcreations.lclib.primitive;

import java.util.function.Function;

@FunctionalInterface
public interface BooleanFunction<R> extends Function<Boolean, R> {
	R apply(boolean b);
	default R apply(Boolean b) {
		return apply(!!b);
	}
}
