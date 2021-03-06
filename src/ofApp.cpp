#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	cout << "Hello World!" << endl;

	// For printing result
    verdana.loadFont("verdana.ttf", 8, false, true);

    /* 
     * ofxASR supports two modes:
     * Mode-4: Any words in the dictionary can be recognized. The 
     *         speaker is allowed to speak anything. In this mode,
     *         it is free to speak but not very accuracy.
     * Mode-2: A list contains some sentences was required and the
     *         speaker must select one sentence to read. If she/he
     *         spoke something not in the list, the engine will 
     *         return the most similar sentence from the list. This
     *         mode seems not very "free" but it is accuracy.
     * The usages of the both two modes are showed below, however, 
     * you only need choose one mode to use.
     */

    //////////////////////////////////////////////////////////////////////////
    // Create a Mode-4 engine
    //////////////////////////////////////////////////////////////////////////
    engine1_listenFromList = new ofxSphinxASR;
    e = new ofAsrEngineArgs;
	
	// set sample rate to 16000 Hz
    e->samplerate = 16000;
	
    // set mode code
    e->sphinx_mode = 4;	

#if defined TARGET_OSX    
    // set the folder of the acoustics model
    e->sphinxmodel_am = ".";

    // set the path of the language model
    e->sphinxmodel_lm = "digit.lm.DMP";

    // set the path of the dict and the filler dict
    e->sphinxmodel_dict = "dictionary";
    e->sphinxmodel_fdict = "fillerdict";
#else
    e->sphinxmodel_am		= ofToDataPath("digit_8gau"		, true);
    e->sphinxmodel_lm		= ofToDataPath("digit.lm.DMP"	, true);
    e->sphinxmodel_dict		= ofToDataPath("digit.dict"		, true);
    e->sphinxmodel_fdict	= ofToDataPath("digit.fdict"	, true);

    //e->sphinxmodel_am		= ofToDataPath("sphinxmodel\digit_8gau"		, true);
    //e->sphinxmodel_lm		= ofToDataPath("sphinxmodel\digit.lm.DMP"	, true);
    //e->sphinxmodel_dict			= ofToDataPath("sphinxmodel\digit.dict"		, true);
    //e->sphinxmodel_fdict	= ofToDataPath("sphinxmodel\digit.fdict"	, true);

	cout << "ofApp::setup() -- e->sphinxmodel_am = " <<  e->sphinxmodel_am << endl;
	cout << "ofApp::setup() -- e->sphinxmodel_lm = " <<  e->sphinxmodel_lm << endl;
	cout << "ofApp::setup() -- e->sphinxmodel_dict = " <<  e->sphinxmodel_dict << endl;
	cout << "ofApp::setup() -- e->sphinxmodel_fdict = " <<  e->sphinxmodel_fdict << endl;
#endif

    // Initial the engine
    int retval = engine1_listenFromList->engineInit(e);
    if (retval != OFXASR_SUCCESS) {
        printf("ASR Engine 1 initial failed. Error Code: %d\n", retval);
		result_from_engine1 = "ASR Engine initial failed. Check sphinx resource path";
    }

    //////////////////////////////////////////////////////////////////////////
    // Create a Mode-2 engine
    //////////////////////////////////////////////////////////////////////////
    engine2_listenFromAnything = new ofxSphinxASR;
    e->sphinx_mode = 2;

    // Mode-2 need a list. Add sentences to the list
	e->sphinx_candidate_sentences.push_back("one");
	e->sphinx_candidate_sentences.push_back("two");
    e->sphinx_candidate_sentences.push_back("three");
	e->sphinx_candidate_sentences.push_back("four");
	e->sphinx_candidate_sentences.push_back("five");
	e->sphinx_candidate_sentences.push_back("six");
	e->sphinx_candidate_sentences.push_back("seven");
	e->sphinx_candidate_sentences.push_back("eight");
	e->sphinx_candidate_sentences.push_back("nine");
	e->sphinx_candidate_sentences.push_back("zero");
    
    retval = engine2_listenFromAnything->engineInit(e);
    if (retval != OFXASR_SUCCESS) {
        printf("ASR Engine 2 initial failed. Error Code: %d\n", retval);
		result_from_engine1 = "ASR Engine initial failed. Check sphinx resource path";
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    if (engine1_listenFromList != NULL) {
        engine1_listenFromList->engineExit();
        delete engine1_listenFromList;
        engine1_listenFromList = NULL;
    }
    if (engine2_listenFromAnything != NULL) {
        engine2_listenFromAnything->engineExit();
        delete engine2_listenFromAnything;
        engine2_listenFromAnything = NULL;
    }
    if (e != NULL) {
        delete e;
        e = NULL;
    }
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(0x000000);
	verdana.drawString("result_from_engine1:", 10,10);
	verdana.drawString(result_from_engine1, 10,30);
	verdana.drawString("result_from_engine2:", 10,70);
	verdana.drawString(result_from_engine2, 10,90);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
		cout << "Hello Again, World!" << endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

	// Open the engines.
    int retval;
    retval = engine1_listenFromList->engineOpen();
    if (retval != OFXASR_SUCCESS) {
        printf("ASR Engine 1 failed to open. Error Code: %d\n", retval);
    }    
    retval = engine2_listenFromAnything->engineOpen();
    if (retval != OFXASR_SUCCESS) {
        printf("ASR Engine 2 failed to open. Error Code: %d\n", retval);
    }

    // Recording. Note that Mac OS do not support 16k sample rate
#if defined TARGET_OSX
	ofSoundStreamSetup(0, 1, this, 48000, 256, 2);
#else
    ofSoundStreamSetup(0, 1, this, 16000, 256, 2);
#endif
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	// Stop recording
    ofSoundStreamClose(); 

    // Close the engines
    int retval;
    retval = engine1_listenFromList->engineClose();
    if (retval != OFXASR_SUCCESS) {
        printf("ASR Engine 1 failed to close. Error Code: %d\n", retval);
    }


    retval = engine2_listenFromAnything->engineClose();
    if (retval != OFXASR_SUCCESS) {
        printf("ASR Engine 2 failed to close. Error Code: %d\n", retval);
    }

    // Get result
	char *result_str;
	result_str = engine1_listenFromList->engineGetText();
	if (result_str) {
		string s1(result_str);
		result_from_engine1 = s1;
	}
	else {
		result_from_engine1 = "[No result.]";
	}

    result_str = engine2_listenFromAnything->engineGetText();
	if (result_str) {
		string s2(result_str);
		result_from_engine2 = s2;
	}
	else {
		result_from_engine2 = "[No result.]";
	}


}		

//--------------------------------------------------------------
void ofApp::audioReceived( float * input, int bufferSize, int nChannels )
{
    if (nChannels > 1) {
        printf("Only support 1-channel audio!\n");
        return;
    }

    // Convert sound from float to short    
#ifdef TARGET_OSX
	short *buf_16 = new short[bufferSize/3];
	for (int i=0; i<bufferSize/3; i++) {
        buf_16[i] = short(input[i*3] * 32767.5 - 0.5);
    }
#else
	short *buf_16 = new short[bufferSize];
    for (int i=0; i<bufferSize; i++) {
        buf_16[i] = short(input[i] * 32767.5 - 0.5);
    }
#endif
	
    // Sent sound to engines
    engine1_listenFromList->engineSentAudio(buf_16, bufferSize);
    engine2_listenFromAnything->engineSentAudio(buf_16, bufferSize);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
