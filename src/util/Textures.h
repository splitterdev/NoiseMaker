#pragma once

class Textures {
    public:
    Textures() {}
    ~Textures() {}

    public:

    static void init( SDL_Renderer* renderer ) {
        _renderer = renderer;
        loadTexture( TEXTURE_BUTTON_GENERAL, "data/textures/ui/general.bmp" );
        loadTexture( TEXTURE_BUTTON_LIBRARY, "data/textures/ui/library.bmp" );
        loadTexture( TEXTURE_BUTTON_DETAILS, "data/textures/ui/details.bmp" );
        loadTexture( TEXTURE_BUTTON_IOCTL, "data/textures/ui/inputControl.bmp" );
        loadTexture( TEXTURE_BUTTON_TEMPLATE_MANAGEMENT, "data/textures/ui/templatemanagement.bmp" );
        loadTexture( TEXTURE_BUTTON_PLAY, "data/textures/ui/play.bmp" );
        loadTexture( TEXTURE_BUTTON_PAUSE, "data/textures/ui/pause.bmp" );
        loadTexture( TEXTURE_BUTTON_RECORD, "data/textures/ui/record.bmp" );
        loadTexture( TEXTURE_BUTTON_RECORDING_NOW, "data/textures/ui/recordingNow.bmp" );
        loadTexture( TEXTURE_BUTTON_SELECTION_TOOL, "data/textures/ui/select.bmp" );
        loadTexture( TEXTURE_BUTTON_LINKING_TOOL, "data/textures/ui/createlink.bmp" );
        loadTexture( TEXTURE_BUTTON_LINKING_INPUT_TOOL, "data/textures/ui/createinputlink.bmp" );
        loadTexture( TEXTURE_BUTTON_LINK_REMOVE_TOOL, "data/textures/ui/deletelink.bmp" );
        loadTexture( TEXTURE_BUTTON_NODE_REMOVE_TOOL, "data/textures/ui/deletenode.bmp" );
        loadTexture( TEXTURE_BUTTON_NODE_MOVE_TOOL, "data/textures/ui/movenode.bmp" );
        loadTexture( TEXTURE_BUTTON_NODE_CREATE, "data/textures/ui/createnode.bmp" );
        loadTexture( TEXTURE_BUTTON_IMPORT_TEMPLATE, "data/textures/ui/import.bmp" );
        loadTexture( TEXTURE_BUTTON_EXPORT_TEMPLATE, "data/textures/ui/export.bmp" );
        loadTexture( TEXTURE_BUTTON_HELP, "data/textures/ui/help.bmp" );
        loadTexture( TEXTURE_BUTTON_SELECT_INPUT, "data/textures/ui/selectInput.bmp" );
        loadTexture( TEXTURE_BUTTON_MOVE_LEFT, "data/textures/ui/moveLeft.bmp" );
        loadTexture( TEXTURE_BUTTON_MOVE_RIGHT, "data/textures/ui/moveRight.bmp" );
        loadTexture( TEXTURE_BUTTON_CANCEL, "data/textures/ui/cancel.bmp" );
        loadTexture( TEXTURE_SPECIAL_OUTPUT, "data/textures/componentlib/output.bmp" );
        loadTexture( TEXTURE_SINEGEN, "data/textures/componentlib/generator/sine.bmp" );
        loadTexture( TEXTURE_TRIANGLEGEN, "data/textures/componentlib/generator/triangle.bmp" );
        loadTexture( TEXTURE_SQUAREGEN, "data/textures/componentlib/generator/square.bmp" );
        loadTexture( TEXTURE_SAWGEN, "data/textures/componentlib/generator/saw.bmp" );
        loadTexture( TEXTURE_CONSTGEN, "data/textures/componentlib/generator/const.bmp" );
        loadTexture( TEXTURE_DOWNWAVEGEN, "data/textures/componentlib/generator/downWave.bmp" );
        loadTexture( TEXTURE_UPWAVEGEN, "data/textures/componentlib/generator/upWave.bmp" );
        loadTexture( TEXTURE_NOISEGEN, "data/textures/componentlib/generator/noise.bmp" );
        loadTexture( TEXTURE_LFNOISEGEN, "data/textures/componentlib/generator/lfnoise.bmp" );
        loadTexture( TEXTURE_TRIANGLENOISEGEN, "data/textures/componentlib/generator/trianglenoise.bmp" );
        loadTexture( TEXTURE_LEVELSIGNALGEN, "data/textures/componentlib/generator/levelsignal.bmp" );
        loadTexture( TEXTURE_ADDER, "data/textures/componentlib/gate/adder.bmp" );
        loadTexture( TEXTURE_MULTIPLIER, "data/textures/componentlib/gate/multiplier.bmp" );
        loadTexture( TEXTURE_ANDGATE, "data/textures/componentlib/gate/and.bmp" );
        loadTexture( TEXTURE_ORGATE, "data/textures/componentlib/gate/or.bmp" );
        loadTexture( TEXTURE_XORGATE, "data/textures/componentlib/gate/xor.bmp" );
        loadTexture( TEXTURE_NOTGATE, "data/textures/componentlib/gate/not.bmp" );
        loadTexture( TEXTURE_INPUT_SELECTOR, "data/textures/componentlib/gate/inputSelector.bmp" );
        loadTexture( TEXTURE_CONSTADDER, "data/textures/componentlib/math/constadder.bmp" );
        loadTexture( TEXTURE_AMPLIFIER, "data/textures/componentlib/math/amplifier.bmp" );
        loadTexture( TEXTURE_DIVIDER, "data/textures/componentlib/math/divider.bmp" );
        loadTexture( TEXTURE_ABSOLUTE, "data/textures/componentlib/math/absolute.bmp" );
        loadTexture( TEXTURE_POWER, "data/textures/componentlib/math/power.bmp" );
        loadTexture( TEXTURE_LOGARITHMIC, "data/textures/componentlib/math/logarithmic.bmp" );
        loadTexture( TEXTURE_DERIVATOR, "data/textures/componentlib/math/derivator.bmp" );
        loadTexture( TEXTURE_INTEGRATOR, "data/textures/componentlib/math/integrator.bmp" );
        loadTexture( TEXTURE_REVERSEPOLARIZATION, "data/textures/componentlib/math/reversepolarization.bmp" );
        loadTexture( TEXTURE_COORDCONVERTER_CTP, "data/textures/componentlib/math/coordCTP.bmp" );
        loadTexture( TEXTURE_COORDCONVERTER_PTC, "data/textures/componentlib/math/coordPTC.bmp" );
        loadTexture( TEXTURE_OVERDRIVE, "data/textures/componentlib/noisectl/fsd.bmp" );
        loadTexture( TEXTURE_BITCRUSHER, "data/textures/componentlib/noisectl/bitCrusher.bmp" );
        loadTexture( TEXTURE_PAIN, "data/textures/componentlib/noisectl/pain.bmp" );
        loadTexture( TEXTURE_HYSTERIA, "data/textures/componentlib/noisectl/hysteria.bmp" );
        loadTexture( TEXTURE_SAMPLESWAPPER, "data/textures/componentlib/noisectl/sampleSwapper.bmp" );
        loadTexture( TEXTURE_INJECTOR, "data/textures/componentlib/noisectl/injector.bmp" );
        loadTexture( TEXTURE_DRAINPUMP, "data/textures/componentlib/noisectl/drainPump.bmp" );
        loadTexture( TEXTURE_SPINLOCKER, "data/textures/componentlib/noisectl/spinLocker.bmp" );
        loadTexture( TEXTURE_BRAID, "data/textures/componentlib/noisectl/braid.bmp" );
        loadTexture( TEXTURE_EQUAL, "data/textures/componentlib/comparator/equal.bmp" );
        loadTexture( TEXTURE_GREATER, "data/textures/componentlib/comparator/greater.bmp" );
        loadTexture( TEXTURE_LESS, "data/textures/componentlib/comparator/less.bmp" );
        loadTexture( TEXTURE_GREATER_OR_EQUAL, "data/textures/componentlib/comparator/greaterOrEqual.bmp" );
        loadTexture( TEXTURE_LESS_OR_EQUAL, "data/textures/componentlib/comparator/lessOrEqual.bmp" );
        loadTexture( TEXTURE_NOT_EQUAL, "data/textures/componentlib/comparator/notEqual.bmp" );
        loadTexture( TEXTURE_MIN, "data/textures/componentlib/comparator/min.bmp" );
        loadTexture( TEXTURE_MAX, "data/textures/componentlib/comparator/max.bmp" );
        loadTexture( TEXTURE_CHANNEL_SELECTOR, "data/textures/componentlib/channelctl/channelSelect.bmp" );
        loadTexture( TEXTURE_CHANNEL_JOINER, "data/textures/componentlib/channelctl/channelJoin.bmp" );
        loadTexture( TEXTURE_CHANNEL_DIFFERENTIATOR, "data/textures/componentlib/channelctl/channelDiff.bmp" );
        loadTexture( TEXTURE_CHANNEL_SWAPPER, "data/textures/componentlib/channelctl/channelSwap.bmp" );
        loadTexture( TEXTURE_CHANNEL_MUTLIPLIER, "data/textures/componentlib/channelctl/channelMul.bmp" );
        loadTexture( TEXTURE_CHANNEL_INVERTER, "data/textures/componentlib/channelctl/channelInv.bmp" );
        loadTexture( TEXTURE_VC_NONE, "data/textures/ui/vcNone.bmp" );
        loadTexture( TEXTURE_VC_OSCILLOSCOPE, "data/textures/ui/oscilloscope.bmp" );
        loadTexture( TEXTURE_VC_SPECTRE, "data/textures/ui/specter.bmp" );
        loadTexture( TEXTURE_VC_SPECTRE_HISTOGRAM, "data/textures/ui/specterHistogram.bmp" );
        loadTexture( TEXTURE_VC_VECTORSCOPE, "data/textures/ui/vector.bmp" );
    }

    static bool loadTexture( int id, const char* path ) {
        SDL_Surface* surface = SDL_LoadBMP( path );
        if ( surface ) {
            SDL_SetColorKey( surface, SDL_TRUE, 0xFFFFFFFF );
            SDL_Texture* texture = SDL_CreateTextureFromSurface( _renderer, surface );
            if ( texture ) {
                SDL_Texture* texturePrev = getTexture( id );
                if ( texturePrev ) {
                    SDL_DestroyTexture( texturePrev );
                }
                SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );
                _textures[ id ] = texture;
                return true;
            }
            SDL_FreeSurface( surface );
        }
        return false;
    }

    static SDL_Texture* getTexture( int id ) {
        map < int, SDL_Texture* >::iterator i = _textures.find( id );
        if ( i != _textures.end() ) {
            return i -> second;
        }
        return NULL;
    }

    private:

    static map < int, SDL_Texture* > _textures;

    static SDL_Renderer* _renderer;

};

map < int, SDL_Texture* > Textures::_textures;
SDL_Renderer* Textures::_renderer = NULL;
