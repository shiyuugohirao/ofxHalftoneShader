//
//  ofxHalftoneShader.h
//
//  Created by shugohirao on 2018/09/17.
//  reffered to http://glslsandbox.com/e#49000.1
//  https://github.com/satoruhiga/ofxIterativeBoxBlur etc.
//

#include "ofMain.h"

class ofxHalftoneShader {
private:
    ofShader shader;
    float dotSize = 10.0;
    float ratio = 5.0;
    float threshold = 0.3; //0.0~1.0
public:
    void setup(){
        setupShader();
    }

    void process(ofFbo& inout) { process(inout.getTexture(), inout); }

    void process(ofTexture& in, ofFbo& out)
    {
        out.begin();
        ofClear(0);
        shader.begin();{
            shader.setUniformTexture("tex0", in, 0);
            shader.setUniform1f("dotSize", dotSize);
            shader.setUniform1f("ratio", ratio);
            shader.setUniform1f("threshold", threshold);
            ofSetColor(255);
            in.draw(0, 0, out.getWidth(), out.getHeight());
        }
        shader.end();
        out.end();
    }

    void setDotSize(float dotSize){ this->dotSize = dotSize; }
    void setRatio(float ratio){ this->ratio = ratio; }
    void setThreshold(float threshold){ this->threshold = threshold; }

    
protected:
    void setupShader(){
        string fShader =
        OS_STRINGIFY(
                     uniform sampler2DRect tex0;
                     uniform float dotSize;
                     uniform float ratio;
                     uniform float threshold;
                     void main (void){
                         vec2 TC = gl_TexCoord[0].xy;
                         float rads = radians(45.);
                         vec2 coord = TC * mat2(cos(rads), sin(rads), -sin(rads), cos(rads));
                         vec2 pos = mod(coord, vec2(dotSize)) - vec2(dotSize/2.0);
                         float d = dot(pos, pos);
                         vec4 col = texture2DRect(tex0, TC);
                         float bri = max(max(col.r,col.g),col.b);
                         //float lum = (col.r+col.g+col.b) /3.; // if you need
                         float v = 1. - bri; // value of strength
                         float show = step(threshold, v);
                         vec4 bCol = vec4(1.,1.,1.,0.);
                         gl_FragColor = mix(col, bCol, smoothstep(v*v*dotSize, v*v*dotSize*ratio, d)) * show;
                     }
                     );

        if(shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fShader)) shader.linkProgram();

        if(!shader.isLoaded()){
            cout<<"[halftoneShader] shader.isLoaded() : failed to load"<<endl;
            std::exit(0);
        }
        else cout<<"[halftoneShader] shader.isLoaded() : success!!"<<endl;
    }

};

