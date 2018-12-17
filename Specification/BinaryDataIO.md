# Binary Data IO #

LCLib Specifies ways that Binary Data is written/read from streams, to allow interoperation between implementations. 


## 1. Base Definitions ##

### 1.1 Byte Order Mode ###

LCLib specifies that both little endianness (native byte order), and big endianness (network byte order) is provided by implementations for binary data IO. In big endian mode, multibyte primitive datatypes are written Most Significant Byte first (So the int value 0x01234567 is written as [01 23 45 67]). In little endian mode, multibyte primitive datatypes are written Least Significant Byte First (so that previous in value is written as [67 45 23 01]). Endianess does not affect the order of fields in composite datatypes (UUIDs are still written Most Significant Long, Least Significant Long in Little Endian Mode), but does affect the byte order of the individual fields. 

LCLib specifies that reading a multibyte primitive datatypes, written by a conforming data output facility may be done validly by a conforming data input facility which is reading in the same Byte Order Mode. There are no restrictions imposed on implementations when multibyte primitive datatypes are read improperly. 

Implementations are required to include in their documentation, which facilities exist to write and read Binary Data, and which Byte Order Mode each facility supports, if not both. Implementations are at least required to provide a facility which supports the Big Endian Byte Order Mode. 
### 1.2 Datatypes ###
LCLib defines the most basic concept for binary data io to be the datatype. There are 2 kinds of datatypes, being primitive datatypes, and composite datatypes. A primitive datatype is one of the basic types. Composite datatypes are built of fields which are themselves datatypes. There are also array types, which represent a context specific number of the base type. 

Implementations are forbidden from adding padding between values written. However users of the implementation may write unused bytes which act as padding. 

#### 1.2.1 Primitive Datatype List ####

LCLib specifies the following primitive datatypes be made availble with the following properties: 
<table>
	<tr>
		<th>Type</th>
		<th>#Bytes</th>
		<th>Description</th>
		<th>Example</th>
		<th>Byte Rep (BE)</th>
		<th>Byte Rep (LE)</th>
	</tr>
	<tr>
		<td>byte(note 1)</td>
		<td>1</td>
		<td>An integer type occupying 8-bits</td>
		<td>0x01</td>
		<td>[01]</td>
		<td>[01]</td>
	</tr>
	<tr>
		<td>short</td>
		<td>2</td>
		<td>An integer type occupying 16-bits</td>
		<td>0x0123</td>
		<td>[01 23]</td>
		<td>[23 01]</td>
	</tr>
	<tr>
		<td>int</td>
		<td>4</td>
		<td>An integer type occupying 32-bits</td>
		<td>0x01234567</td>
		<td>[01 23 45 67]</td>
		<td>[67 45 23 01]</td>
	</tr>
	<tr>
		<td>long</td>
		<td>8</td>
		<td>An integer type occupying 64-bits</td>
		<td>0x0123456789ABCDEF</td>
		<td>[01 23 45 67 89 AB CD EF]</td>
		<td>[EF CD AB 89 67 45 23 01]</td>
	</tr>
	<tr>
		<td>float(note 2)</td>
		<td>4</td>
		<td>A floating point type occupying 32-bits.</td>
		<td>1.1f</td>
		<td>[3F 8C CC CD]</td>
		<td>[CD CC 8C 3F]</td>
	</tr>
	<tr>
		<td>double (note 2)</td>
		<td>8</td>
		<td>A floating point type occupying 64-bits.</td>
		<td>1.1</td>
		<td>[3F F1 99 99 99 99 99 9A]</td>
		<td>[9A 99 99 99 99 99 F1 3F]</td>
	</tr>
</table>
(1): Because the byte type only occupies 1 full byte, it is unaffected by the Byte Order Mode. Because of this, a byte which is written by a conforming data output facility may be read by any conforming data input facility, reguardless of the byte order mode of either facility. 

(2): LCLib mandates that the binary representation of float and double MUST following the IEC559 (IEEE754) Floating Point standard in binary32 (for float) or binary64 (for double) specification. However LCLib does not mandate that the internal representation of the value be this format. Therefore precision loss may occur when the target implementation uses a different floating-point representation. Other specifications however, may impose additional restrictions, so it is recommended that you use that particular floating point representation. Neither qNaN values nor sNaN values may be written by conforming data output facilities (this restriction only applies to writing floating-point values, conforming input facilities may choose to error upon reading such a value, discard the value silently, or return the NaN as read). Both +Infinity and -Infinity are valid values to be written. 


#### 1.2.2 Composite Datatype List ####

In addition to the various primitive type, LCLib also specifies a number of composite types, which are made up of multiple fields, each of a specific datatype. The ordering of fields in the composite type is mandated by the specification, and is unaffected by Byte Order Mode. However the ordering of the bytes which make up multibyte primitive fields of these composite datatypes are affected by the Byte Order Mode. 

##### 1.2.2.1 String #####

Strings are the basic type of text in LCLib. Strings are written with a 2-byte (short) length prefix, followed by that many bytes (not characters). Strings are to be encoded in UTF-8. Strings are not Terminated by a null byte, and MAY NOT contain embedded null bytes. 

String Structure:

```
string{
	short length;
	byte codePoints[length];
}
```

Note: the length of a string is to be treated as unsigned (with the range 0-65535).

##### 1.2.2.2 Version #####

Versions represent a Major and Minor version code in the Lightning Creations Versioning Format. LCLib versions contain a major and minor field. The major field can contain a value from 1-256 and the minor field may contain a value for 0-255. The written major field actually contains the major version-1 so that it can fit in a byte. The minor version field is written as is.

Version Structure:

```
version{
	byte major;
	byte minor;
}
```

##### 1.2.2.3 UUID #####

UUIDs are 16-byte Universally Unique Identifiers, which are suitable for Identifying various kinds of objects. 

UUID Structure:

```
uuid{
	long most;
	long least;
}
```

##### 1.2.2.4 Instant #####

Instants are a Point in time since the Unix Epoch (Midnight on January 1st, 1970 UTC). They store the number of seconds since this point, and the number of nanoseconds since the beginning of the reported second. 

The seconds field of an Instant must be in the range (-31556889864401400,31556889864401400], and the nanos field must be in the range (0,1000000000]. The instant described by a 0 seconds field and a 0 nanos field is the unix epoch. 

Instant Structure:

```
instant{
	long seconds;
	int nanos;
};
```

##### 1.2.2.5 Duration #####

Durations represent a period of time. Similar to an instant, they store the length of time as a seconds field and a nanoseconds field. These fields must fall in the ranges specified. Note that a Duration is not an Instant. There definitions, and therefore stored values of there types are incompatible. 

```
duration{
	long seconds;
	int nanos;
};
```


