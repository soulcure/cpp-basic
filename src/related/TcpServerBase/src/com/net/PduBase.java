package com.net;

public class PduBase {
	/****************************************************
	 * basic unit of data type length 
	 */
	public static final int pdu_basic_length = 4;
	/****************************************************
	 * index 0. pos:[0-4)
	 * the begining flag of a pdu.
	 */
	public static final int startflag = 123456789 ;
	
	/****************************************************
	 * index 1. pos:[4-8)
	 */
	public int terminal_token /*user_id*/ ;
	/****************************************************
	 * index 2. pos:[8-12)
	 */
	public int commandid ;
	/****************************************************
	 * index 3. pos:[12-16)
	 */
	public int seq_id ;
	/****************************************************
	 * index 4. pos:[16-20)
	 */
	public int length ;
	
	/***************************************************
	 * index 5. pos:[20-infinity)
	 */
	public byte[] body ;
}


