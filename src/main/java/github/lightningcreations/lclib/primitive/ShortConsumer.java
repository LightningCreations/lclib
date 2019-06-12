package github.lightningcreations.lclib.primitive;

import java.util.function.Consumer;
import java.util.function.IntConsumer;

@FunctionalInterface
public interface ShortConsumer extends Consumer<Short> {
	void accept(short s);
	default void accept(Short s) {
		accept(s.shortValue());
	}
	default ShortConsumer andThen(ShortConsumer next) {
		return val->{ShortConsumer.this.accept(val);next.accept(val);};
	}
	default IntConsumer toUnsigned() {
		return val->ShortConsumer.this.accept((short)val);
	}
	static ShortConsumer fromUnsigned(IntConsumer v) {
		return val->v.accept(val&0xffff);
	}
}
