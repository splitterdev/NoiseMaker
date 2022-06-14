#pragma once

#include <stdlib.h>
#include <math.h>
#include <time.h>

#define TEXTURE_BUTTON_GENERAL 501
#define TEXTURE_BUTTON_LIBRARY 502
#define TEXTURE_BUTTON_DETAILS 503
#define TEXTURE_BUTTON_IOCTL 504
#define TEXTURE_BUTTON_TEMPLATE_MANAGEMENT 505

#define TEXTURE_BUTTON_PLAY 601
#define TEXTURE_BUTTON_PAUSE 602
#define TEXTURE_BUTTON_RECORD 603

#define TEXTURE_BUTTON_RECORDING_NOW 901
#define TEXTURE_BUTTON_SELECTION_TOOL 902
#define TEXTURE_BUTTON_LINKING_TOOL 903
#define TEXTURE_BUTTON_LINKING_INPUT_TOOL 904
#define TEXTURE_BUTTON_LINK_REMOVE_TOOL 905
#define TEXTURE_BUTTON_NODE_REMOVE_TOOL 906
#define TEXTURE_BUTTON_NODE_MOVE_TOOL 907
#define TEXTURE_BUTTON_NODE_CREATE 908
#define TEXTURE_BUTTON_IMPORT_TEMPLATE 909
#define TEXTURE_BUTTON_EXPORT_TEMPLATE 910
#define TEXTURE_BUTTON_HELP 911
#define TEXTURE_BUTTON_SELECT_INPUT 912
#define TEXTURE_BUTTON_MOVE_LEFT 913
#define TEXTURE_BUTTON_MOVE_RIGHT 914
#define TEXTURE_BUTTON_CANCEL 915

#define TEXTURE_SPECIAL_OUTPUT 999

#define TEXTURE_SINEGEN 1001
#define TEXTURE_TRIANGLEGEN 1002
#define TEXTURE_SQUAREGEN 1003
#define TEXTURE_SAWGEN 1004
#define TEXTURE_CONSTGEN 1005
#define TEXTURE_DOWNWAVEGEN 1006
#define TEXTURE_UPWAVEGEN 1007

#define TEXTURE_NOISEGEN 1501
#define TEXTURE_LFNOISEGEN 1502
#define TEXTURE_TRIANGLENOISEGEN 1503
#define TEXTURE_LEVELSIGNALGEN 1504

#define TEXTURE_ADDER 2001
#define TEXTURE_MULTIPLIER 2002
#define TEXTURE_CONSTADDER 2003
#define TEXTURE_AMPLIFIER 2004
#define TEXTURE_DIVIDER 2005
#define TEXTURE_POWER 2006
#define TEXTURE_LOGARITHMIC 2007
#define TEXTURE_ABSOLUTE 2008
#define TEXTURE_DERIVATOR 2009
#define TEXTURE_INTEGRATOR 2010
#define TEXTURE_REVERSEPOLARIZATION 2011
#define TEXTURE_COORDCONVERTER_CTP 2012
#define TEXTURE_COORDCONVERTER_PTC 2013

#define TEXTURE_NOTGATE 2501
#define TEXTURE_ANDGATE 2502
#define TEXTURE_ORGATE 2503
#define TEXTURE_XORGATE 2504
#define TEXTURE_EQUAL 2505
#define TEXTURE_GREATER 2506
#define TEXTURE_LESS 2507
#define TEXTURE_GREATER_OR_EQUAL 2508
#define TEXTURE_LESS_OR_EQUAL 2509
#define TEXTURE_NOT_EQUAL 2510
#define TEXTURE_MIN 2511
#define TEXTURE_MAX 2512
#define TEXTURE_CHANNEL_SELECTOR 2513
#define TEXTURE_CHANNEL_JOINER 2514
#define TEXTURE_CHANNEL_DIFFERENTIATOR 2515
#define TEXTURE_CHANNEL_SWAPPER 2516
#define TEXTURE_CHANNEL_MUTLIPLIER 2517
#define TEXTURE_CHANNEL_INVERTER 2518
#define TEXTURE_INPUT_SELECTOR 2519

#define TEXTURE_OVERDRIVE 3001
#define TEXTURE_BITCRUSHER 3002

#define TEXTURE_PAIN 4001
#define TEXTURE_HYSTERIA 4002
#define TEXTURE_SAMPLESWAPPER 4003
#define TEXTURE_INJECTOR 4004
#define TEXTURE_DRAINPUMP 4005
#define TEXTURE_SPINLOCKER 4006
#define TEXTURE_BRAID 4007

#define TEXTURE_VC_NONE 10000
#define TEXTURE_VC_OSCILLOSCOPE 10001
#define TEXTURE_VC_SPECTRE 10002
#define TEXTURE_VC_SPECTRE_HISTOGRAM 10003
#define TEXTURE_VC_VECTORSCOPE 10004


#include "../util/Textures.h"
#include "Buffer.h"
#include "ParameterControlRule.h"
#include "ChainingNode.h"

#include "processor/AdderNode.h"
#include "processor/MultiplierNode.h"
#include "processor/DividerNode.h"
#include "processor/InputSelectorNode.h"

#include "ChainingLine.h"
#include "ChainingTree.h"
#include "ChainPlayer.h"

#include "processor/AmplifierNode.h"
#include "processor/AbsoluteNode.h"
#include "processor/ConstAdderNode.h"
#include "generator/SineGenerator.h"
#include "generator/TriangleGenerator.h"
#include "generator/SquareGenerator.h"
#include "generator/SawGenerator.h"
#include "generator/ConstGenerator.h"
#include "generator/DownWaveGenerator.h"
#include "generator/UpWaveGenerator.h"
#include "generator/NoiseGenerator.h"
#include "generator/NoiseLFGenerator.h"
#include "generator/TriangleNoiseGenerator.h"
#include "generator/LevelSignalGenerator.h"
#include "processor/FSDNode.h"
#include "processor/ExponentNode.h"
#include "processor/LogarithmicNode.h"
#include "processor/DerivatorNode.h"
#include "processor/IntegratorNode.h"
#include "processor/CartesianToPolarNode.h"
#include "processor/PolarToCartesianNode.h"
#include "processor/ReversePolarizationNode.h"

#include "processor/OrNode.h"
#include "processor/AndNode.h"
#include "processor/XorNode.h"
#include "processor/NotNode.h"

#include "comparator/EqualNode.h"
#include "comparator/LessNode.h"
#include "comparator/GreaterNode.h"
#include "comparator/LessOrEqualNode.h"
#include "comparator/GreaterOrEqualNode.h"
#include "comparator/NotEqualNode.h"
#include "comparator/MaxNode.h"
#include "comparator/MinNode.h"

#include "noisectl/PainNode.h"
#include "noisectl/HysteriaNode.h"
#include "noisectl/SampleSwapperNode.h"
#include "noisectl/InjectorNode.h"
#include "noisectl/BitcrusherNode.h"
#include "noisectl/DrainPumpNode.h"
#include "noisectl/SpinLockerNode.h"
#include "noisectl/BraidNode.h"

#include "channelctl/ChannelSelectorNode.h"
#include "channelctl/ChannelJoinerNode.h"
#include "channelctl/ChannelDifferentiatorNode.h"
#include "channelctl/ChannelSwapperNode.h"
#include "channelctl/ChannelMultiplierNode.h"
#include "channelctl/ChannelInverterNode.h"

#include "ChainingNodeFactory.h"
#include "../template/AlgorithmTemplate.h"


#define CLICKMODE_SELECT 0
#define CLICKMODE_ADD 1
#define CLICKMODE_LINKING 2
#define CLICKMODE_LINK_REMOVE 3
#define CLICKMODE_NODE_REMOVE 4
#define CLICKMODE_NODE_MOVE 5
#define CLICKMODE_LINKING_INPUT 6

int clickMode = CLICKMODE_SELECT;


#include "../gui/Button.h"
#include "../gui/Edit.h"
#include "../gui/ParamEdit.h"
#include "../gui/StaticText.h"
#include "../gui/Panel.h"
#include "../gui/TabPanel.h"
#include "../gui/LibrarySelectorButton.h"
#include "../gui/PlayButton.h"
#include "../gui/PauseButton.h"
#include "../gui/RecordButton.h"
#include "../gui/ChangeClickModeButton.h"
#include "../gui/ChangeTabButton.h"

#include "../gui/DialogSelectorBox.h"
#include "../gui/TabbedDataSelectorBox.h"
#include "../gui/InputSelectDialogSelectorBox.h"
#include "../gui/ImportTemplateSelector.h"
#include "../gui/MessageDialogSelectorBox.h"
#include "../gui/ExportTemplateButton.h"
#include "../gui/HelpButton.h"

#include "../gui/DialogLauncher.h"
#include "../gui/ImportTemplateButton.h"
#include "../gui/ParamDialogLauncher.h"

#include "FFTVisualisationHelper.h"

#include "../gui/VisualisationComponent.h"
#include "../gui/VisualisationComponentSelector.h"
#include "../gui/VisualisationComponentSelectorLauncher.h"

#include "../gui/VisualisationComponentFactories.h"

