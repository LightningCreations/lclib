package github.lightningcreations.lclib;

import java.lang.reflect.InvocationTargetException;

public interface Callable<T> {
	public Object call(T obj,Object... args)throws InvocationTargetException;
}
