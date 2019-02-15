package github.lightningcreations.lclib;

import java.security.SecureRandom;
import java.time.Duration;
import java.time.Instant;
import java.util.UUID;
import java.util.concurrent.atomic.AtomicInteger;

public class UUIDs {
	private static SecureRandom rand = new SecureRandom();
	private static final AtomicInteger clockSeq = new AtomicInteger(65867);
	private static final Instant UUID_EPOCH = Instant.ofEpochSecond(-12219292800L);
	public static UUID genTimebasedUUID() {
		int seq = clockSeq.get();
		while(!clockSeq.compareAndSet(seq, 65537*seq+1))seq=clockSeq.get();
		
		Instant ts = Instant.now();
		Duration count = Duration.between(UUID_EPOCH, ts);
		long tm = count.getSeconds()*10000000+count.getNano();
		long high = (tm&0xFFFFFFFF)<<32|(tm&0xFFFF00000000L)>>32|(tm&0xfff000000000000L)>>48|0x1000;
		long low = ((0x8000L|seq&0x3fff)<<48)|(rand.nextLong()>>17)|0x800000000000L;
		return new UUID(high,low);
	}
}
