
//报警时频率锁定函数Freq_Lock()

void Freq_Lock()
{
  void sweep(void)
{
   Uint16 i=0;
   Uint32 freq;
   //freq=0x4000;
   //freq=0x4225;//1.675MHz
   freq=0x4AE5;//8.5MHz   

   for(i=0;i<16;i++)
      {
	    mcbspb_xmit(freq,0);
        //gpio1315162930();
		
	    //gpio2930();
	    /*
	    delayxus(1000);
        mcbspb_xmit(freq,0);
	    delayxus(1000);
		mcbspb_xmit(freq,0);
	    delayxus(1000);
		mcbspb_xmit(freq,0);
	    delayxus(1000);
		mcbspb_xmit(freq,0);
	    delayxus(1000);
	    mcbspb_xmit(freq,0);
	    delayxus(1000);
		mcbspb_xmit(freq,0);
	    delayxus(1000);
		mcbspb_xmit(freq,0);
	    delayxus(1000);
		mcbspb_xmit(freq,0);
	    delayxus(1000);
		mcbspb_xmit(freq,0);
	    delayxus(1000);
		*/
		freq=freq-0x000B;
      }
}
}



