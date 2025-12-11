/*
{	"na"      : 	"n/a"		   }	,   {	"desc"	: 	"n/a"									                }
{	"servo"   : 	"0 to 100" }	,   {	"desc"	: 	"Set Servo Position as %"				      }
{	"home"    : 	"null"		 }  ,   {	"desc"	: 	"Home the Servo (pos 0)"				      }
{	"tare"    : 	"null"		 }	,   {	"desc"	: 	"Zero encoder position & Load Cell"		}
{	"sample"  : 	"1 to 200" }  ,   {	"desc"	: 	"not implemented"		                  }
{	"print"   : 	"1 to 200" }	,   { "desc"	: 	"not implemented"	                  	}
{	"stream"  : 	"null"		 }	,	  { "desc"	: 	"Start Data Streaming"					      }
{	"endst"   : 	"null"		 }	,	  { "desc"	: 	"End Data Streaming"				        	}
{	"secret"  : 	"char[8]"	 }	,	  { "desc"	:	  "set the secret word"				        	}
{	"getcal"  : 	"null"		 }	,   { "desc"	:	  "get calibration from memory"		    	}
{ "recall"  :   "null"     }	,   { "desc"	:	  "get settings data from memory"		   	}
{	"info"    : 	"null"		 }	,	  { "desc"	:	  "print settings, cal data & validity "}
{	"help"    : 	"null"		 }	,	  { "desc"	:	  "print commands list"									}

// Auth CMDs
{	"cal"     : "integer" 	,	"auth"	:	"char[8]"	 } , { "desc" : "save load cell cal to mem"	  }
{	"setmat"  : "char[16]"	,	"auth"	:	"char[8]"	 } , {	"desc" : "set material"						      }
{	"setdia"  : "char[8]"	  ,	"auth"	:	"char[8]"	 } , {	"desc" : "set diameter"						      }
{	"setang"  : "uint"		  ,	"auth"	:	"char[8]"	 } , {	"desc" : "set angle max"					      }
{	"setload" : "int"		    ,	"auth"	:	"char[8]"  } , {	"desc" : "set load max"						      }


*/



void print_cmds() {
  Serial.println(F("{\"na     \" : \"n/a\", \"desc\" : \"n/a\"}"));
  Serial.println(F("{\"servo  \" : \"0 to 100\", \"desc\" : \"Set Servo Position as %\"}"));
  Serial.println(F("{\"home   \" : \"null\", \"desc\" : \"Home the Servo (pos 0) \"}"));
  Serial.println(F("{\"tare   \" : \"null\", \"desc\" : \"Zero encoder position & Load Cell \"}"));
  Serial.println(F("{\"sample \" : \"1 to 200 \", \"desc\" : \"Set Samplerate in Hz (dflt: 1)\"}"));
  Serial.println(F("{\"print  \" : \"1 to 200 \", \"desc\" : \"Set Print Rate in Hz (dflt: 1) \"}"));
  Serial.println(F("{\"stream \" : \"null\", \"desc\" : \"Start Data Streaming  \"}"));
  Serial.println(F("{\"endst  \" : \"null \", \"desc\" : \"End Data Streaming  \"}"));
  Serial.println(F("{\"secret \" : \"char[8]\", \"desc\" : \"set the secret word  \"}"));
  Serial.println(F("{\"cal    \" : \"integer ,\"auth\":\"char[8]\"\", \"desc\" : \"save load cell cal to memy\"}"));
  Serial.println(F("{\"getcal \" : \"null\", \"desc\" : \"get calibration from memory \"}"));
  Serial.println(F("{\"setmat \" : \"char[16],\"auth\":\"char[8]\", \"desc\" : \"set material  \"}"));
  Serial.println(F("{\"setdia \" : \"char[8],\"auth\\\":\"char[8]\"  , \"desc\" : \"set diameter\"}"));
  Serial.println(F("{\"setang \" : \"uint,\"auth\":\"char[8]\"     \", \"desc\" : \"set angle max\"}"));
  Serial.println(F("{\"setload\" : \"int,\"auth\":\"char[8]\"      \", \"desc\" : \"set load max  \"}"));
  Serial.println(F("{\"recall\" :  \"null\", \"desc\" : \"get settings from memory \"}"));
  Serial.println(F("{\"info   \" : \"null\", \"desc\" : \"print settings, cal data & validity \"}"));
  Serial.println(F("{\"help   \" : \"null\", \"desc\" : \"print commands list\"}"));
}
