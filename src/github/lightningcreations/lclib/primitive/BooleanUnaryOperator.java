package github.lightningcreations.lclib.primitive;

import java.util.function.UnaryOperator;

@FunctionalInterface
public interface BooleanUnaryOperator extends UnaryOperator<Boolean> {
	public boolean apply(boolean b);
	default Boolean apply(Boolean b) {
		return apply(!!b);
	}
	default BooleanUnaryOperator chain(BooleanUnaryOperator o) {
		return b->o.apply(BooleanUnaryOperator.this.apply(b));
	}
	static BooleanUnaryOperator identity() {
		return b->b;
	}
	static BooleanUnaryOperator negation() {
		return b->!b;
	}
}
