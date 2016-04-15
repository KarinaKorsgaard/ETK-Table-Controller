#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    
    receiver.setup(9000);
    sender.setup(HOST, 5000);
    
    string newClipboardContent = ofSystemTextBoxDialog("type name of sequence (name of folder in data you are using!)");
    ofxClipboard::copy(newClipboardContent); //<<<<<<<<<<<<<<<<<
    dataFolder = ofxClipboard::paste(); //<<<<<<<<<<<<<<<<<
    string path = dataFolder;
    
    ofDirectory dir(dataFolder);
    if(!dir.exists()){
        string newClipboardContent = ofSystemTextBoxDialog("there is no folder with that name - write something else");
        ofxClipboard::copy(newClipboardContent); //<<<<<<<<<<<<<<<<<
        dataFolder = ofxClipboard::paste(); //<<<<<<<<<<<<<<<<<
        string path = dataFolder;
    }
    
    for(int i = 0; i<12;i++){
        bool b = false;
        whoAreConnected.push_back(b);
        int o = 1;
        disable.push_back(o);
    }
    tablenames.push_back("/tableOne/alive");
    tablenames.push_back("/tableTwo/alive");
    tablenames.push_back("/tableThree/alive");
    tablenames.push_back("/tableFour/alive");
    tablenames.push_back("/tableFive/alive");
    tablenames.push_back("/tableSix/alive");
    tablenames.push_back("/tableSeven/alive");
    tablenames.push_back("/tableEight/alive");
    tablenames.push_back("/tableNine/alive");
    tablenames.push_back("/tableTen/alive");
    tablenames.push_back("/tableEleven/alive");
    tablenames.push_back("/tableTwelve/alive");
    
    gui.setup();
    gui.add(allOff.setup("C - table connection"));
    gui.add(updateScene.setup("U - update"));
    gui.add(startScene.setup("S - start the scene",false));
    gui.add(pause.setup("P - pause",false));
    gui.add(showResults.setup("R -show Results",false));
    gui.add(nextStage.setup("N - next stage", false));
    //gui.add(nextQuestion.setup("N - next question",false));
    gui.add(clearPoint.setup("X - clear points",false));
    gui.add(tableMode.setup("T - tableMode",false));
    gui.add(showChart.setup("V - view top 10", false));
    gui.add(slider.setup("top 10", 0, 0, 25));
    gui.add(averaged.setup("A - averaged/added",true));    //gui.add(endScene.setup("end the scene",false));
    gui.add(grid.setup("G - toggle Grid", true));
    gui.add(simulator.setup("simulate"));
    //gui.add(ise.setup("win/loose, scene 3", false));
    if( xml.loadFile(dataFolder+"/data.xml") ){
        cout <<"loaded" + dataFolder+"/data.xml"<<endl;
    }
    xml.pushTag("document");
    int scenes = xml.getNumTags("scene");
    for(int i = 0; i<scenes; i++){
        
        xml.pushTag("scene",i);
        sceneNames.push_back(xml.getValue("file",""));
        ofxXmlSettings sceneXml;
        sceneXml.loadFile(dataFolder+"/"+xml.getValue("file",""));
        
        sceneXml.pushTag("document");
        if(sceneXml.tagExists("assesment")){
            sceneXml.pushTag("assesment");
            howmanystages.push_back(sceneXml.getNumTags("question"));
            sceneXml.popTag();
        }
        if(sceneXml.tagExists("quiz")){
            sceneXml.pushTag("quiz");
            howmanystages.push_back(sceneXml.getNumTags("question"));
            sceneXml.popTag();
            
        }
        if(sceneXml.tagExists("vote")){
            howmanystages.push_back(0);
            
        }
        sceneXml.popTag();
        xml.popTag();
    }
    xml.popTag();

    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(updateXml){
        updateXml = false;
        xml.pushTag("document");
        int scenes = xml.getNumTags("scene");
        sceneNames.clear();
        howmanystages.clear();
        for(int i = 0; i<scenes; i++){
            
            xml.pushTag("scene",i);
            sceneNames.push_back(dataFolder+"/"+xml.getValue("file",""));
            ofxXmlSettings sceneXml;
            sceneXml.loadFile(dataFolder+"/"+xml.getValue("file",""));
            
            sceneXml.pushTag("document");
            if(sceneXml.tagExists("assesment")){
                sceneXml.pushTag("assesment");
                howmanystages.push_back(sceneXml.getNumTags("question"));
                sceneXml.popTag();
            }
            if(sceneXml.tagExists("quiz")){
                sceneXml.pushTag("quiz");
                howmanystages.push_back(sceneXml.getNumTags("question"));
                sceneXml.popTag();
                
            }
            if(sceneXml.tagExists("vote")){
                howmanystages.push_back(0);
                
            }
            sceneXml.popTag();
            xml.popTag();
        }
        xml.popTag();

    }
    
    while(receiver.hasWaitingMessages()){
        ofxOscMessage msg;
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        for(int i = 0; i<tablenames.size();i++){
            if(m.getAddress() == tablenames[i]){
                whoAreConnected[i] = true;
            }
        }
        
        msg.setAddress(m.getAddress());
        
        for(int i = 0; i<m.getNumArgs();i++){
            if(m.getArgType(i)==3){
                msg.addFloatArg(m.getArgAsFloat(i));
            }
            if(m.getArgType(i)==1){
                msg.addIntArg(m.getArgAsInt32(i));
            }
        }
      
        sender.sendMessage(msg);
        m.clear();
    }
    if(allOff==1){
        for(int i = 0; i<12;i++){
            
            whoAreConnected[i] = false;
            
        }
    }
    
    ofxOscMessage msg;
    
    
    
    if(prevScene!=scene||sendAll){
        prevScene=scene;
        ofxOscMessage msg;
        msg.setAddress("/scene");
        msg.addIntArg(scene);
        sender.sendMessage(msg);
        sendAll = true;
        stageCounter = 0;
        startScene = false;
        showResults = 0;
        startScene = 0;
    }
    
    if(updateScene||up||sendAll){
        up = false;
        msg.clear();
        msg.setAddress("/updateScene");
        sender.sendMessage(msg);
    }
    if(nextStage==1 ){
        nextStage = 0;
        stageCounter++;
        
    }
    
    if(prevStageCounter != stageCounter||sendAll){
        prevStageCounter = stageCounter;
        msg.clear();
        msg.setAddress("/stageNumber");
        msg.addIntArg(stageCounter);
        sender.sendMessage(msg);
        showResults = 0;
        showResutlsFuckCrap = true;
    }
    if(prewShowResults!=showResults||sendAll||showResutlsFuckCrap){
        msg.clear();
        msg.setAddress("/showResults");
        msg.addIntArg(showResults);
        sender.sendMessage(msg);
        prewShowResults = showResults;
        showResutlsFuckCrap = false;
        
    }

    
    
    if(prevStartScene!=startScene ){
        prevStartScene = startScene;
        msg.clear();
        msg.setAddress("/startScene");
        msg.addIntArg(startScene);
        sender.sendMessage(msg);
        
        //startScene = 0;
    }
 
    if(clearPoint==1||sendAll){
        clearPoint = 0;
        msg.clear();
        msg.setAddress("/clearPoint");
        sender.sendMessage(msg);
    }
    if(simulator){
        
        msg.clear();
        msg.setAddress("/simulator");
        sender.sendMessage(msg);
    }

    if(grid!=prevGrid||sendAll){
        
        msg.clear();
        msg.setAddress("/grid");
        msg.addIntArg(grid);
        sender.sendMessage(msg);
        prevGrid = grid;
    }

    if(averaged!=prevAveraged||sendAll){
        prevAveraged = averaged;
        msg.clear();
        msg.setAddress("/averaged");
        msg.addIntArg(averaged);
        sender.sendMessage(msg);
    }
    
    
    
    

    if(prevSlider!=slider||sendAll){
        msg.clear();
        msg.setAddress("/slider");
        msg.addIntArg(slider);
        sender.sendMessage(msg);
        prevSlider = slider;
    }
    if(prevTableToggle != tableMode||sendAll){
        
        if(tableMode==1){
            msg.clear();
            msg.setAddress("/tableMode");
            msg.addIntArg(1);
            sender.sendMessage(msg);
        }
        else if(tableMode==0){
            msg.clear();
            msg.setAddress("/tableMode");
            msg.addIntArg(0);
            sender.sendMessage(msg);
        }
        prevTableToggle = tableMode;
    }
    
    
    
    if(sendDisable||sendAll){
        msg.clear();
        msg.setAddress("/disable");
        
        for(int i = 0 ; i<disable.size();i++){
            int send;
            if(disable[i]){
                send = 1;
            }
            if(!disable[i]){
                send = 0;
            }
            msg.addIntArg(send);
        }
        sender.sendMessage(msg);
        sendDisable = false;
    }
    
    if(prevShowChart != showChart||sendAll){
        
        if(showChart==1){
            ofxOscMessage msg;
            msg.setAddress("/showChart");
            msg.addIntArg(1);
            sender.sendMessage(msg);
        }
        else if(showChart==0){
            ofxOscMessage msg;
            msg.setAddress("/showChart");
            msg.addIntArg(0);
            sender.sendMessage(msg);
        }
        prevShowChart = showChart;
    }
    
    if(prevShowChart != showChart||sendAll){
        
        if(showChart==1){
            ofxOscMessage msg;
            msg.setAddress("/showChart");
            msg.addIntArg(1);
            sender.sendMessage(msg);
        }
        else if(showChart==0){
            ofxOscMessage msg;
            msg.setAddress("/showChart");
            msg.addIntArg(0);
            sender.sendMessage(msg);
        }
        prevShowChart = showChart;
    }
    
    
    if(prevPause != pause||sendAll){
        if(pause==1){
            ofxOscMessage msg;
            msg.setAddress("/pause");
            msg.addIntArg(1);
            sender.sendMessage(msg);
        }
        else if(pause==0){
            ofxOscMessage msg;
            msg.setAddress("/pause");
            msg.addIntArg(0);
            sender.sendMessage(msg);
        }
        prevPause = pause;
    }
    
    
    guiX = gui.getWidth();
    guiY = gui.getHeight();
    guiP = gui.getPosition();
    sendAll = false;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(0, 255, 255);
    ofDrawRectangle(guiP.x,guiP.y, guiX, guiY);
    gui.draw();
    ofSetColor(255);
    ofDrawBitmapString("scene: "+ofToString(scene+1), guiP.x,guiP.y+guiY+15);
    
    for(int i = 0; i<12;i++){
        
        if(whoAreConnected[i]){
            ofSetColor(40, 255, 50);
        }
        else if(!whoAreConnected[i]){
            ofSetColor(255, 40, 50);
        }
        ofFill();
        
        if(i<=5){
        
            ofDrawRectangle(guiP.x, 30*i+guiY+guiP.y+30, 30, 30);
            ofNoFill();
            ofSetLineWidth(4);
            ofSetColor(0);
            ofDrawRectangle(guiP.x, 30*i+guiY+guiP.y+30, 30, 30);
            ofDrawBitmapString(ofToString(i+1),guiP.x+10,guiY+guiP.y+50+30*i);
        }
        if(i>5){
            
            ofDrawRectangle(guiP.x+40, 30*(i-6)+guiY+guiP.y+30, 30, 30);
            ofNoFill();
            ofSetLineWidth(4);
            ofSetColor(0);
            ofDrawRectangle(guiP.x+40, 30*(i-6)+guiY+guiP.y+30, 30, 30);
            ofDrawBitmapString(ofToString(i+1),guiP.x+50,guiY+guiP.y+50+30*(i-6));
        }
    }
    
     for(int i = 0; i<sceneNames.size();i++){
        // ofRect(250, 20, 50, 50*i+50);
         if(i==scene){
             ofFill();
             ofSetColor(180);
         }
         ofDrawRectangle(guiX+30, guiP.y+i*50, 50, 50);
         ofSetColor(255);
         ofNoFill();
         ofDrawRectangle(guiX+30, guiP.y+i*50, 50, 50);
         ofDrawBitmapString(ofToString(i+1),guiX+30+20, 50*i+guiP.x+30);
         
         ofDrawBitmapString(sceneNames[i], guiX+30+20+70,guiP.y+i*50+30);
         
     
     }
    
    
    
    for(int i = 0 ; i<howmanystages[scene]+1;i++){
        int xx =guiX+30+60;
        int yy =guiP.y+scene*50+i*20;
        
        ofNoFill();
        ofSetLineWidth(1);
        ofSetColor(255);
        ofDrawRectangle(xx,yy, 20,20);
        if(i==howmanystages[scene]){
            ofFill();
            ofSetColor(255);
            ofDrawLine(xx,yy,xx+20,yy+20);
            
            ofDrawLine(xx+20,yy,xx,yy+20);
        }
        if(stageCounter==i){
            ofFill();
            ofSetColor(255, 0, 0,180);
            ofDrawRectangle(xx,yy, 20,20);
        }
        

    }
    for(int i = 0; i<6;i++){
        for(int u = 0; u<2;u++){
            int xx=guiP.x+40+u*50+50;
            int yy=guiY+guiP.y+30+i*50;
            if(disable[i+6*u]){
                ofSetColor(0,255,0,190);
            }
            else if(!disable[i+6*u]){
                ofSetColor(255,0,0,190);
            }
            
            
            ofFill();
            ofDrawRectangle(xx, yy, 50, 50);
            ofSetColor(255);
            ofDrawBitmapString(ofToString(i+6*u +1), xx+20,yy+30);
            ofNoFill();
            ofSetLineWidth(1);
            ofSetColor(255);
            ofDrawRectangle(xx,yy, 50,50);
        }
    }
    
    ofNoFill();
    ofSetColor(255);
    ofDrawRectangle(ofGetWidth()-50, ofGetHeight()-50, 40, 40);
    
    ofSetColor(255, 255, 0);
    ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 5);
    
    
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if(key=='c'){
        allOff!=allOff;
    }
    if(key=='s'){
        startScene=1;
    }
    if(key=='n'){
        nextStage=1;
    }
    if(key=='g'){
        grid= (grid*-1)+1;
    }
    if(key=='r'){
        showResults=1;
    }
    if(key=='p'){
        pause=(pause*-1)+1;
    }
    
    if(key=='x'){
        clearPoint=1;
    }
    if(key=='t'){
        tableMode=(tableMode*-1) + 1;
    }
    if(key=='u'){
        up=true;
    }
    if(key=='v'){
        showChart= (showChart*-1)+1;
    }
    if(key=='a'){
        averaged= (averaged*-1)+1;
    }
    for(int i = 0; i<10;i++){
        if(key==i+49){
            scene=i;
        }
    }
    if(key==48){
        scene=9;
    }
    cout<<key<<endl;

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    cout<<ofToString(x)+"-"+ofToString(y)<<endl;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

    for(int i = 0; i<sceneNames.size();i++){
        int xx = guiX+30;
        int yy = guiP.y+i*50;
        if(x>xx&&x<xx+50){
            if(y>yy&&y<yy+50){
               
                
                scene=i;
                
                ofFill();
                int xx =guiX+30+70;
                int yy =guiP.y+scene*50;
                ofSetColor(255, 0, 0);
                ofDrawRectangle(xx,yy, 20,20);
                
            }
        }
 
    }
    
    for(int i = 0 ; i<howmanystages[scene]+1;i++){
        int xx =guiX+30+60;
        int yy =guiP.y+scene*50+i*20;
        if(x>xx&&x<xx+20){
            if(y>yy&&y<yy+20){
            
                stageCounter=i;
                
            }
        }
    }

    for(int i = 0; i<6;i++){
        for(int u = 0; u<2;u++){
            int xx=guiP.x+40+u*50+50;
            int yy=guiY+guiP.y+30+i*50;
            if(x>xx&&x<xx+50){
                if(y>yy&&y<yy+50){
                    cout<<"im hit"<<endl;
                    if(disable[i+u*6] == false){
                        disable[i+u*6]=true;
                    }
                    else if(disable[i+u*6] == true){
                        disable[i+u*6]=false;
                    }
                    sendDisable = true; 
                }
            }
            
        }
    }
    
 
    if(x>ofGetWidth()-50&&y>ofGetHeight()-50){
        updateXml=true;
    }
    
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
