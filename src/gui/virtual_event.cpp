/**
 *  VIRTUAL_EVENT.CPP
 *
 *  This file is part of ROGUELIKETHING.
 *
 *  ROGUELIKETHING is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROGUELIKETHING is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ROGUELIKETHING.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "virtual_event.h"
VirtualEvent::VirtualEvent() {
}

VirtualEvent::~VirtualEvent() {
    //Do nothing
}

void VirtualEvent::OnEvent(SDL_Event* Event) {
    switch(Event->type) {
        case SDL_ACTIVEEVENT: {
            switch(Event->active.state) {
                case SDL_APPMOUSEFOCUS: {
                    if ( Event->active.gain ) {
                        OnMouseFocus();
                    } else {
                        OnMouseBlur();
                    }
                    break;
                }
                case SDL_APPINPUTFOCUS: {
                    if ( Event->active.gain ) {
                        OnInputFocus();
                    } else {
                        OnInputBlur();
                    }
                    break;
                }
                case SDL_APPACTIVE: {
                    if ( Event->active.gain ){
                        OnRestore();
                    } else {
                        OnMinimize();
                    }
                    break;
                }
            }
            break;
        }

        case SDL_KEYDOWN: {
            OnKeyDown(Event->key.keysym.sym,Event->key.keysym.mod,Event->key.keysym.unicode);
            break;
        }

        case SDL_KEYUP: {
            OnKeyUp(Event->key.keysym.sym,Event->key.keysym.mod,Event->key.keysym.unicode);
            break;
        }

        case SDL_MOUSEMOTION: {
            OnMouseMove(Event->motion.x,Event->motion.y,Event->motion.xrel,Event->motion.yrel,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_LEFT))!=0,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT))!=0,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE))!=0);
            break;
        }

        case SDL_MOUSEBUTTONDOWN: {
            switch(Event->button.button) {
                case SDL_BUTTON_LEFT: {
                    OnLButtonDown(Event->button.x,Event->button.y);
                    break;
                }
                case SDL_BUTTON_RIGHT: {
                    OnRButtonDown(Event->button.x,Event->button.y);
                    break;
                }
                case SDL_BUTTON_MIDDLE: {
                    OnMButtonDown(Event->button.x,Event->button.y);
                    break;
                }
            }
            break;
        }

        case SDL_MOUSEBUTTONUP: {
            switch(Event->button.button) {
                case SDL_BUTTON_LEFT: {
                    OnLButtonUp(Event->button.x,Event->button.y);
                    break;
                }
                case SDL_BUTTON_RIGHT: {
                    OnRButtonUp(Event->button.x,Event->button.y);
                    break;
                }
                case SDL_BUTTON_MIDDLE: {
                    OnMButtonUp(Event->button.x,Event->button.y);
                    break;
                }
            }
            break;
        }

        case SDL_JOYAXISMOTION: {
            OnJoyAxis(Event->jaxis.which,Event->jaxis.axis,Event->jaxis.value);
            break;
        }

        case SDL_JOYBALLMOTION: {
            OnJoyBall(Event->jball.which,Event->jball.ball,Event->jball.xrel,Event->jball.yrel);
            break;
        }

        case SDL_JOYHATMOTION: {
            OnJoyHat(Event->jhat.which,Event->jhat.hat,Event->jhat.value);
            break;
        }
        case SDL_JOYBUTTONDOWN: {
            OnJoyButtonDown(Event->jbutton.which,Event->jbutton.button);
            break;
        }

        case SDL_JOYBUTTONUP: {
            OnJoyButtonUp(Event->jbutton.which,Event->jbutton.button);
            break;
        }

        case SDL_QUIT: {
            OnExit();
            break;
        }

        case SDL_SYSWMEVENT: {
            //Ignore
            break;
        }

        case SDL_VIDEORESIZE: {
            OnResize(Event->resize.w,Event->resize.h);
            break;
        }

        case SDL_VIDEOEXPOSE: {
            OnExpose();
            break;
        }

        default: {
            OnUser(Event->user.type,Event->user.code,Event->user.data1,Event->user.data2);
            break;
        }
    }
}

void VirtualEvent::OnInputFocus() {
    //Pure virtual, do nothing
}

void VirtualEvent::OnInputBlur() {
    //Pure virtual, do nothing
}

void VirtualEvent::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    //Pure virtual, do nothing
}

void VirtualEvent::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    //Pure virtual, do nothing
}

void VirtualEvent::OnMouseFocus() {
    //Pure virtual, do nothing
}

void VirtualEvent::OnMouseBlur() {
    //Pure virtual, do nothing
}

void VirtualEvent::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle) {
    //Pure virtual, do nothing
}

void VirtualEvent::OnMouseWheel(bool Up, bool Down) {
    //Pure virtual, do nothing
}

void VirtualEvent::OnLButtonDown(int mX, int mY) {
    //Pure virtual, do nothing
}

void VirtualEvent::OnLButtonUp(int mX, int mY) {
    //Pure virtual, do nothing
}

void VirtualEvent::OnRButtonDown(int mX, int mY) {
    //Pure virtual, do nothing
}

void VirtualEvent::OnRButtonUp(int mX, int mY) {
    //Pure virtual, do nothing
}

void VirtualEvent::OnMButtonDown(int mX, int mY) {
    //Pure virtual, do nothing
}

void VirtualEvent::OnMButtonUp(int mX, int mY) {
    //Pure virtual, do nothing
}

void VirtualEvent::OnJoyAxis(Uint8 which,Uint8 axis,Sint16 value) {
    //Pure virtual, do nothing
}

void VirtualEvent::OnJoyButtonDown(Uint8 which,Uint8 button) {
    //Pure virtual, do nothing
}

void VirtualEvent::OnJoyButtonUp(Uint8 which,Uint8 button) {
    //Pure virtual, do nothing
}

void VirtualEvent::OnJoyHat(Uint8 which,Uint8 hat,Uint8 value) {
    //Pure virtual, do nothing
}

void VirtualEvent::OnJoyBall(Uint8 which,Uint8 ball,Sint16 xrel,Sint16 yrel) {
    //Pure virtual, do nothing
}

void VirtualEvent::OnMinimize() {
    //Pure virtual, do nothing
}

void VirtualEvent::OnRestore() {
    //Pure virtual, do nothing
}

void VirtualEvent::OnResize(int w,int h) {
    //Pure virtual, do nothing
}

void VirtualEvent::OnExpose() {
    //Pure virtual, do nothing
}

void VirtualEvent::OnExit() {
    //Pure virtual, do nothing
}

void VirtualEvent::OnUser(Uint8 type, int code, void* data1, void* data2) {
    //Pure virtual, do nothing
}
