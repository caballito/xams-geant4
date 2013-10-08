// Short macro to count Compton double scatters inside
// lXe cubes of various edge lengths.
{
	int count[21] = {} ;
	count[0] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-150 && xp>-50 && xp<50 && yp>-50 && yp<50","colz") ;
	count[1] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-155 && xp>-47.5 && xp<47.5 && yp>-47.5 && yp<47.5","colz") ;
	count[2] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-160 && xp>-45 && xp<45 && yp>-45 && yp<45","colz") ;
	count[3] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-165 && xp>-42.5 && xp<42.5 && yp>-42.5 && yp<42.5","colz") ;
	count[4] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-170 && xp>-40 && xp<40 && yp>-40 && yp<40","colz") ;
	count[5] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-175 && xp>-37.5 && xp<37.5 && yp>-37.5 && yp<37.5","colz") ;
	count[6] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-180 && xp>-35 && xp<35 && yp>-35 && yp<35","colz") ;
	count[7] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-185 && xp>-32.5 && xp<32.5 && yp>-32.5 && yp<32.5","colz") ;
	count[8] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-190 && xp>-30 && xp<30 && yp>-30 && yp<30","colz") ;
	count[9] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-195 && xp>-27.5 && xp<27.5 && yp>-27.5 && yp<27.5","colz") ;
	count[10] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-200 && xp>-25 && xp<25 && yp>-25 && yp<25","colz") ;
	count[11] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-205 && xp>-22.5 && xp<22.5 && yp>-22.5 && yp<22.5","colz") ;
	count[12] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-210 && xp>-20 && xp<20 && yp>-20 && yp<20","colz") ;
	count[13] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-215 && xp>-17.5 && xp<17.5 && yp>-17.5 && yp<17.5","colz") ;
	count[14] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-220 && xp>-15 && xp<15 && yp>-15 && yp<15","colz") ;
	count[15] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-224.6 && xp>-12.7 && xp<12.7 && yp>-12.7 && yp<12.7","colz") ;
	count[16] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-225 && xp>-12.5 && xp<12.5 && yp>-12.5 && yp<12.5","colz") ;
	count[17] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-230 && xp>-10 && xp<10 && yp>-10 && yp<10","colz") ;
	count[18] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-235 && xp>-7.5 && xp<7.5 && yp>-7.5 && yp<7.5","colz") ;
	count[19] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-240 && xp>-5 && xp<5 && yp>-5 && yp<5","colz") ;
	count[20] = events->Draw("steplen:zp","creaproc.data()==\"compt\" && edproc.data()==\"compt\" && zp>-250 && zp<-245 && xp>-2.5 && xp<2.5 && yp>-2.5 && yp<2.5","colz") ;

	for (int i=0;i<21;i++)
		cout << i << "\t" << count[i] << endl ;

}
