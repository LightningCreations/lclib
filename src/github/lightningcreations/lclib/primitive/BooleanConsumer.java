package github.lightningcreations.lclib.primitive;

import java.util.function.Consumer;

@FunctionalInterface
public interface BooleanConsumer extends Consumer<Boolean> {
	void accept(boolean val);
	default void accept(Boolean val) {
		accept(val.booleanValue());
	}
	default BooleanConsumer andThen(final BooleanConsumer next) {
		return v->{accept(v);next.accept(v);};
	}
	default BooleanConsumer transform(BooleanUnaryOperator op) {
		return v->accept(op.apply(v));
	}
	static BooleanConsumer select(BooleanPredicate selector,BooleanConsumer act1,BooleanConsumer act2) {
		return b->{
			if(selector.test(b))
				act1.accept(b);
			else
				act2.accept(b);
		};
	}
	static BooleanConsumer nop() {
		return b->{};
	}
	/**
	 * Returns a BooleanConsumer that ignores its argument, and applies the given action
	 */
	static BooleanConsumer ignoresArguement(Runnable r) {
		return b->r.run();
	}
}
