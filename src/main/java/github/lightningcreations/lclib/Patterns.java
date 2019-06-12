package github.lightningcreations.lclib;

public interface Patterns {
	String identifier = "[A-Za-z_][\\w_]*",
			qidentifier = "[A-Za-z_][\\\\w_]*(\\.[A-Za-z_][\\\\w_]*)*",
			version = "([1-9] {1}|[0-9]{2}|2[0-4][0-9]|25[0-6])\\.([0-9]{1,2}|2[0-4][0-9]|25[0-5)",
			uuid = "[0-9A-Fa-f] {8}-[0-9A-Fa-f] {4}-[0-9A-Fa-f] {4}-[0-9A-Fa-f] {4}-[0-9A-Fa-f] {12}";
}
