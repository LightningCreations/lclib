package github.lightningcreations.lclib.primitive;

import java.util.function.BinaryOperator;

@FunctionalInterface
public interface BooleanBinaryOperator extends BinaryOperator<Boolean> {
	public boolean apply(boolean a,boolean b);
	default Boolean apply(Boolean a,Boolean b) {
		return apply(!!a,!!b);
	}
	static BooleanBinaryOperator and() {
		return (a,b)->a&&b;
	}
	static BooleanBinaryOperator or() {
		return (a,b)->a||b;
	}
	static BooleanBinaryOperator xor() {
		return (a,b)->(a&&!b)||(!a&&b);
	}
}
