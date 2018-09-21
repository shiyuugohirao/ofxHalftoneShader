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
    float dotInterval = 10.0;   // interval
    float dotSize = 8.0;        // diameter(dotInterval = maxSize)
    float aaRate = 1.05;        // AntiAliasingRate
    float threshold = 0.3;      // threshold of depth (0.0~1.0)
    bool useLuminance = false;  // use (true)Luminace / (false)Brightness

public:
    void setup(){
        setupShader();
    }

    //[*] deprecated (Under construction)
    //void process(ofFbo& inout) { process(inout.getTexture(), inout); }

    void process(ofTexture& in, ofFbo& out)
    {
        out.begin();
        ofClear(0);
        shader.begin();{
            shader.setUniformTexture("tex0", in, 0);
            shader.setUniform1f("dotInterval", dotInterval);
            shader.setUniform1f("dotSize", dotSize);
            shader.setUniform1f("aaRate", aaRate);
            shader.setUniform1f("threshold", threshold);
            shader.setUniform1i("useLuminance", (int)useLuminance);
            ofSetColor(255);
            in.draw(0, 0, out.getWidth(), out.getHeight());
        }
        shader.end();
        out.end();
    }

    void setDotInterval(float dotInterval){ this->dotInterval = dotInterval; }
    void setDotSize(float dotSize){ this->dotSize = dotSize; }
    void setAaRate(float aaRate){ this->aaRate = aaRate; }
    void setThreshold(float threshold){ this->threshold = threshold; }
    void setUseLuminance(bool b){ useLuminance = b; }


protected:
    void setupShader(){
        string fShader =
        OS_STRINGIFY(
                     uniform sampler2DRect tex0;
                     uniform float dotInterval;
                     uniform float dotSize;
                     uniform float aaRate;
                     uniform float threshold;
                     uniform int useLuminance;
                     void main (void){
                         vec2 TC = gl_TexCoord[0].xy;
                         float rads = radians(45.);
                         float radius = dotSize * 0.5; // diameter/2
                         vec2 coord = TC * mat2(cos(rads), sin(rads), -sin(rads), cos(rads));
                         vec2 pos = mod(coord, vec2(dotInterval)) - vec2(dotInterval/2.) + vec2(radius);
                         float radPos = length(pos-vec2(radius));
                         vec4 col = texture2DRect(tex0, TC);

                         float bri = max(max(col.r,col.g),col.b);
                         float lum = (col.r + col.g + col.b) / 3.;
                         float bl = mix(bri, lum, float(useLuminance));
                         float depth = 1. - bl;

                         float show = step(threshold, depth);

                         vec4 bCol = vec4(1.,1.,1.,0.);
                         float rate = smoothstep(pow(depth,2.)*dotSize, pow(depth,2.)*dotSize*aaRate, radPos);
                         gl_FragColor = mix(col, bCol, rate) * show;
                     }
                     );

        if(shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fShader)) shader.linkProgram();

        if(!shader.isLoaded()){
            cout<<"[ofxHalftoneShader] shader.isLoaded() : failed to load"<<endl;
            std::exit(0);
        }
        else cout<<"[ofxHalftoneShader] shader.isLoaded() : success!!"<<endl;
    }

};

