package github.lightningcreations.lclib;

import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.stream.Stream;


public class LookupClassLoader extends ClassLoader implements ClassLookup {

	
	public final Stream<Class<?>> listLoadedClasses(){
		return ClassLookup.getClasses(this.getParent());
	}

}
