#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"


namespace das {

    struct SimPrint : SimVisitor {
        SimPrint ( TextWriter & wr ) : ss(wr) {
        }
        virtual void preVisit ( SimNode * node ) override {
            xcr.push_back(CR);
            CR = false;
            tab ++;
            SimVisitor::preVisit(node);
            ss << "(";
        }
        virtual SimNode * visit ( SimNode * node ) override {
            ss << ")";
            tab --;
            CR = xcr.back();
            xcr.pop_back();
            return SimVisitor::visit(node);
        }
        virtual void cr () override {
            CR = true;
        }
        virtual void op ( const char * name ) override {
            SimVisitor::op(name);
            ss << name;
        }
        virtual void sp ( uint32_t stackTop, const char * name ) override {
            SimVisitor::sp(stackTop,name);
            ss << " #" << stackTop;
        }
        virtual void arg ( int32_t argV,  const char * argN ) override {
            SimVisitor::arg(argV,argN);
            ss << " " << argV;
        }
        virtual void arg ( uint32_t argV,  const char * argN ) override {
            SimVisitor::arg(argV,argN);
            ss << " " << argV;
        }
        virtual void arg ( const char * argV,  const char * argN ) override {
            SimVisitor::arg(argV,argN);
            if ( argV ) {
                ss << " \"" << argV << "\"";
            } else {
                ss << " null";
            }
        }
        virtual void arg ( uint64_t argV,  const char * argN ) override {
            SimVisitor::arg(argV,argN);
            ss << " " << argV;
        }
        virtual void arg ( bool argV,  const char * argN ) override {
            SimVisitor::arg(argV,argN);
            ss << " " << (argV ? "true" : "false");
        }
        virtual void arg ( vec4f argV,  const char * argN ) override {
            SimVisitor::arg(argV,argN);
            union {
                uint32_t    ui[4];
                vec4f       v;
            } X; X.v = argV;
            ss << " (" << X.ui[0] << "," << X.ui[1] << "," << X.ui[2] << "," << X.ui[3] << ")";
        }

        virtual void sub ( SimNode ** nodes, uint32_t count, const char * ) override {
            if ( CR ) {
                ss << "\n" << string(tab,'\t');
            } else {
                ss << " ";
            }
            ss << "(";
            for ( uint32_t t = 0; t!=count; ++t ) {
                tab ++;
                if ( t ) {
                    ss << "\n" << string(tab,'\t');
                }
                nodes[t] = nodes[t]->visit(*this);
                tab --;
            }
            ss << ")";
        }
        virtual SimNode * sub ( SimNode * node, const char * opN  ) override {
            if ( CR ) {
                ss << "\n" << string(tab,'\t');
            } else {
                ss << " ";
            }
            SimNode * res = SimVisitor::sub(node,opN);
            return res;
        }
        TextWriter & ss;
        int tab = 0;
        bool CR = false;
        vector<bool> xcr;
    };

    void printSimNode ( TextWriter & ss, SimNode * node ) {
        SimPrint prv(ss);
        node->visit(prv);
    }

}
