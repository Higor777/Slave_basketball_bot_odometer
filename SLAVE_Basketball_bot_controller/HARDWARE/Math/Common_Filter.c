

/*
   �޷��˲��㷨 
*/
typedef float ALF;  // �˲�����������

ALF AmplitudeLimiterFilter(ALF new_value , ALF A)   // ���뵱ǰ�ɼ�ֵ ���޷�����   ������Чֵ
{   
	
  static ALF value;    
	if ( ( new_value - value > A ) || ( value - new_value > A ))  
	return value;              // ������һ����Чֵ
	else
	{
     value=new_value;       //�����ϴ���Чֵ������	
		 return value;  
  }
      
}   

/*	
	 ��λֵ�˲��� 
	 ��������N�Σ�����С����ȡ�м�ֵΪ��Чֵ
	 ����ð�ݷ�
*/

typedef float MIDF;  // �˲�����������
#define MidN  11   
MIDF MiddlevalueFilter(MIDF new_value[MidN]) //�������������n��ֵ��������ֵ
{   
		MIDF value_buf[MidN];   
		char count,i,j,temp;   
		for ( count=0;count<MidN;count++)   
		{   
				value_buf[count] = new_value[count];   	
		}   
		for (j=0;j<MidN-1;j++)   
		{   
				for (i=0;i<MidN-j;i++)   
				{   
						if ( value_buf[i]>value_buf[i+1] )   
						{   
								temp = value_buf[i];   
								value_buf[i] = value_buf[i+1];    
								value_buf[i+1] = temp;   
						}   
				}   
		}   
		return value_buf[(MidN-1)/2];   
}        

/*
  ����ƽ���˲��㷨
*/	

typedef float AAF;  // �˲�����������
#define AAN 12   
AAF Arithmetic_Average_F(AAF new_value[AAN])   
{   
		AAF  sum = 0;   
	  char count;
		for ( count=0;count<AAN;count++)   
		{  
			
				sum += new_value[count];   
			  
		}   
		return sum/AAN;   
}   

		
/*	

   ����ƽ���˲� 
   ��֮ǰN��ֵȡƽ��
   
*/
typedef float SAF;  // �˲�����������
#define SAN 12   

char Slide_Average_filter(SAF new_value)   
{   
		static SAF value_buf[SAN],sum=0;   
		static char i=0,count=0;     
		value_buf[i++] =  new_value;   
		if ( i == SAN )   i = 0;   
		for ( count=0;count<SAN;count++)   
				sum += value_buf[count];   
		return sum/SAN;   
}   

/*
		
    #define N 12   
    char filter()   
    {   
        char count,i,j;   
        char value_buf[N];   
        int  sum=0;   
        for  (count=0;count<N;count++)   
        {   
            value_buf[count] = get_ad();   
            delay();   
        }   
        for (j=0;j<N-1;j++)   
        {   
            for (i=0;i<N-j;i++)   
            {   
                if ( value_buf[i]>value_buf[i+1] )   
                {   
                    temp = value_buf[i];   
                    value_buf[i] = value_buf[i+1];    
                    value_buf[i+1] = temp;   
                }   
            }   
        }   
        for(count=1;count<N-1;count++)   
            sum += value[count];   
        return (char)(sum/(N-2));   
    }   

/*****

		
    #define a 50   
    char value;   
    char filter()   
    {   
        char  new_value;   
        new_value = get_ad();   
        return (100-a)*value + a*new_value;    
    }   

		
		
		
    #define N 12   
    char code coe[N] = {1,2,3,4,5,6,7,8,9,10,11,12};   
    char code sum_coe = 1+2+3+4+5+6+7+8+9+10+11+12;   
    char filter()   
    {   
        char count;   
        char value_buf[N];   
        int  sum=0;   
        for (count=0,count<N;count++)   
        {   
            value_buf[count] = get_ad();   
            delay();   
        }   
        for (count=0,count<N;count++)   
            sum += value_buf[count]*coe[count];   
        return (char)(sum/sum_coe);   
    }   

		
		
    #define N 12   
    char filter()   
    {   
        char count=0;   
        char new_value;   
        new_value = get_ad();   
        while (value !=new_value);   
        {   
            count++;   
            if (count>=N)   return new_value;   
            delay();   
            new_value = get_ad();   
        }   
        return value;       
    }   
		****/
