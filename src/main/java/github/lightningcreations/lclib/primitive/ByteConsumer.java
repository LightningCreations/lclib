package github.lightningcreations.lclib.primitive;

import java.util.function.Consumer;
import java.util.function.IntConsumer;

public interface ByteConsumer extends Consumer<Byte> {
	void accept(byte val);
	default void accept(Byte val) {
		accept(val.byteValue());
	}
	default ByteConsumer andThen(ByteConsumer next) {
		return val->{ByteConsumer.this.accept(val);next.accept(val);};
	}
	default IntConsumer toUnsigned() {
		return val->ByteConsumer.this.accept((byte)val);
	}
	static ByteConsumer fromUnsigned(IntConsumer v) {
		return val->v.accept(val&0xff);
	}
}
