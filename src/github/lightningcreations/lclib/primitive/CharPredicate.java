package github.lightningcreations.lclib.primitive;

import java.util.function.Predicate;

@FunctionalInterface
public interface CharPredicate extends Predicate<Character> {
	public boolean test(char c);
	default boolean test(Character c) {
		return test(c.charValue());
	}
	default CharPredicate and(CharPredicate next) {
		return c->CharPredicate.this.test(c)&&next.test(c);
	}
	default CharPredicate or(CharPredicate next) {
		return c->CharPredicate.this.test(c)||next.test(c);
	}
	default CharPredicate negate() {
		return c->!CharPredicate.this.test(c);
	}
	static CharPredicate letter() {
		return c->Character.isLetter(c);
	}
	static CharPredicate whitespace() {
		return c->Character.isWhitespace(c);
	}
	static CharPredicate digit() {
		return c->Character.isDigit(c);
	}
	static CharPredicate identifierStart() {
		return c->Character.isJavaIdentifierStart(c);
	}
	static CharPredicate identifierPart() {
		return c->Character.isJavaIdentifierPart(c);
	}
	static CharPredicate isValid() {
		return c->Character.isDefined(c);
	}
	static CharPredicate isSurrogate() {
		return c->Character.isSurrogate(c);
	}
	static CharPredicate isPrimary() {
		return c->!Character.isSurrogate(c);
	}
	static CharPredicate alwaysTrue() {
		return c->true;
	}
	static CharPredicate alwaysFalse() {
		return c->false;
	}
}
