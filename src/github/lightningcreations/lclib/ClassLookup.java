package github.lightningcreations.lclib;

import java.net.URL;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Stream;

public interface ClassLookup {
	public static Stream<Class<?>> getClasses(ClassLoader ctxLoader){
		try {
			List<Class<?>> list = new ArrayList<Class<?>>();
			if(ctxLoader==null)
				ctxLoader = ClassLoader.getPlatformClassLoader();
			Package[] ps = ctxLoader.getDefinedPackages();
			for(Package p:ps) {
				String name = p.getName().replaceAll("\\.", "\\");
				Path pkgSubpath = Paths.get(name);
				URL pkgURL = ctxLoader.getResource(name);
				Path pkgPath = Paths.get(pkgURL.toURI());
				for(URL s:Iterables.fromEnumeration(ctxLoader.getResources(name))) {
					Path lPath = Paths.get(s.toURI()).relativize(pkgPath).resolve(pkgSubpath);
					if(!lPath.endsWith(".class")||lPath.endsWith("package-info.class"))
						continue;
					String clName = pkgPath.toString().replaceAll("[\\\\/]", ".");
					clName = clName.substring(0, clName.length()-6);
					if(clName.startsWith("."))
						clName = clName.substring(1);
					Class<?> cl = Class.forName(clName, false, ctxLoader);
					list.add(cl);
				}
			}
			return list.stream();
		}catch(Exception e) {
			return Stream.empty();
		}
	}
	public static Stream<Class<?>> getClasses(){
		return getClasses(Thread.currentThread().getContextClassLoader());
	}
	
}
