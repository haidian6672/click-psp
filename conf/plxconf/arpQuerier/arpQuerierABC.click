c0::Classifier(12/0806 16/6000 20/0002, -);
FromDevice(ens33)->c0;
out::Queue(100)->todevice0::ToDevice(ens33);
arpq0::ARPQuerierABC(16.16.16.16.16.16.16.16, 00:0c:29:9d:0f:af)->out;
c0[0]->[1]arpq0;
FastPSPSource(2, 10, 128, 00:0c:29:9d:0f:af, 0x1010101010101010, 2345, 
              ff:ff:ff:ff:ff:ff, 0x1111111111111111, 2346) 
      -> Unqueue(100)
      -> Strip(14)
      -> [0]arpq0
      -> out;
c0[1]->Discard;
