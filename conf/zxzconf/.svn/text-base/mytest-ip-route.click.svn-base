// mytest-ip-route.click
// eth0 202.197.12.4 00:0c:29:cb:72:14    //00:00:C0:3B:71:EF
// eth1 11.1.12.4 00:0c:29:cb:72:1e    //00:00:C0:CA:68:EF

define($DEV eth0, $DEV1 eth1,$DADDR 202.197.12.190, $GW 202.197.12.1)

ip :: Print("Forward IP Pkt") -> Strip(14)
    -> CheckIPHeader(INTERFACES 202.197.12.4/255.255.255.0 11.1.12.4/255.255.255.0)
    -> rt :: StaticIPLookup(
	202.197.12.4/32 0,
	202.197.12.255/32 0,
	202.197.12.0/32 0,
	11.1.12.4/32 0,
	11.1.12.255/32 0,
	11.1.12.0/32 0,
	202.197.12.0/255.255.255.0 1,
	11.1.12.0/255.255.255.0 2,
	255.255.255.255/32 0.0.0.0 0,
	0.0.0.0/32 0,
	0.0.0.0/0.0.0.0 18.26.4.1 1);

c0 :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800);
out0 :: Queue(200) -> todevice0 :: ToDevice(eth0);
arpq0 :: ARPQuerier(202.197.12.4, 00:0c:29:cb:72:14) -> out0;

FromDevice(eth0) -> c0;
c0[0]->ar0 :: ARPResponder(202.197.12.4 00:0c:29:cb:72:14) -> out0;
c0[1]->[1]arpq0;
c0[2]->ip;

c1 :: Classifier(12/0806 20/0001, 12/0806 20/0002, 12/0800);
out1 :: Queue(200) -> todevice1 :: ToDevice(eth1);
arpq1 :: ARPQuerier(11.1.12.4, 00:0c:29:cb:72:1e) -> out1;

FromDevice(eth1) ->c1;
c1[0]->ar1 :: ARPResponder(11.1.12.4 00:0c:29:cb:72:1e) -> out1;
c1[1]->[1]arpq1;
c1[2]->ip;

rt[0]->Discard;
rt[3]->Discard;
rt[1] -> DropBroadcasts
    -> cp0 :: PaintTee(1)
    -> gio0 :: IPGWOptions(202.197.12.4)
    -> FixIPSrc(202.197.12.4)
    -> dt0 :: DecIPTTL
    -> fr0 :: IPFragmenter(1500)
    -> Print(rt1)
    -> [0]arpq0;
dt0[1] -> ICMPError(202.197.12.4, timeexceeded) -> rt;
fr0[1] -> ICMPError(202.197.12.4, unreachable, needfrag) -> rt;
gio0[1] -> ICMPError(202.197.12.4, parameterproblem) -> rt;
cp0[1] -> ICMPError(202.197.12.4, redirect, host) -> rt;

// Forwarding path for eth1
rt[2] -> DropBroadcasts
    -> cp1 :: PaintTee(2)
    -> gio1 :: IPGWOptions(11.1.12.4)
    -> FixIPSrc(11.1.12.4)
    -> dt1 :: DecIPTTL
    -> fr1 :: IPFragmenter(1500)
    -> Print(rt2)
    -> [0]arpq1;

dt1[1] -> ICMPError(11.1.12.4, timeexceeded) -> rt;
fr1[1] -> ICMPError(11.1.12.4, unreachable, needfrag) -> rt;
gio1[1] -> ICMPError(11.1.12.4, parameterproblem) -> rt;
cp1[1] -> ICMPError(11.1.12.4, redirect, host) -> rt;

