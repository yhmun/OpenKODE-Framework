/*
 * SIPR decoder for the 16k mode
 *
 * Copyright (c) 2008 Vladimir Voroshilov
 * Copyright (c) 2009 Vitor Sessak
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVCODEC_SIPR16KDATA_H
#define AVCODEC_SIPR16KDATA_H

static const float pred_16k[2] = {0.8f, 0.6f};
static const float qu[2] = { 0.12f, 0.5f};

static const float gain_cb_16k[32] = {
    0.07499f,  0.10593f,  0.14125f,  0.18836f,
    0.23714f,  0.28184f,  0.32734f,  0.37584f,
    0.42170f,  0.47315f,  0.53088f,  0.59566f,
    0.66834f,  0.74989f,  0.84140f,  0.94406f,
    1.05925f,  1.18850f,  1.33352f,  1.49624f,
    1.67880f,  1.88365f,  2.11349f,  2.37137f,
    2.66073f,  3.05492f,  3.54813f,  4.21697f,
    5.30884f,  7.07946f,  9.44061f, 13.33521f,
};

static const float gain_pitch_cb_16k[16] = {
    0.00f, 0.2f, 0.40f, 0.5f, 0.60f, 0.7f, 0.75f, 0.8f,
    0.85f, 0.9f, 0.95f, 1.0f, 1.05f, 1.1f, 1.15f, 1.2f,
};


static const float mean_lsf_16k[16] = {
    0.131554f,  0.246615f,  0.435896f,  0.644419f,
    0.827810f,  1.017876f,  1.198910f,  1.379159f,
    1.562157f,  1.736908f,  1.940719f,  2.131963f,
    2.347162f,  2.521521f,  2.717870f,  2.847068f
};

/**
 * Hamming windowed sinc function, like in AMR
 */
static const float sinc_win[40] = {
     0.874475f,  0.755101f,  0.455962f,  0.118807f, -0.114223f, -0.176778f,
    -0.101923f,  0.015553f,  0.086555f,  0.078193f,  0.018660f, -0.037513f,
    -0.052733f, -0.027459f,  0.009967f,  0.030278f,  0.024050f,  0.003055f,
    -0.013862f, -0.016162f, -0.006725f,  0.004212f,  0.008634f,  0.005721f,
    -0.000000f, -0.003710f, -0.003690f, -0.001228f,  0.001409f,  0.002610f,
};

static const float lsf_cb1_16k[128][3] = {
    {-0.089990f, -0.172485f, -0.203391f}, {-0.094710f, -0.178687f, -0.134483f},
    {-0.056398f, -0.131952f, -0.154500f}, {-0.051362f, -0.128138f, -0.198549f},
    {-0.061700f, -0.142830f, -0.251623f}, {-0.041512f, -0.115637f, -0.229420f},
    {-0.036544f, -0.107512f, -0.173125f}, {-0.024158f, -0.088450f, -0.204144f},
    {-0.038690f, -0.103368f, -0.132674f}, {-0.056954f, -0.128472f, -0.104669f},
    {-0.020963f, -0.076785f, -0.163199f}, {-0.012952f, -0.077249f, -0.128385f},
    {-0.032787f, -0.097044f, -0.093967f}, {-0.035214f, -0.053838f, -0.111940f},
    {-0.013850f, -0.036926f, -0.139328f}, {-0.004956f, -0.065092f, -0.087709f},
    {-0.065354f, -0.065595f, -0.079064f}, {-0.023627f, -0.081457f, -0.054195f},
    {-0.027335f, -0.035244f, -0.068034f}, { 0.016555f, -0.047075f, -0.128286f},
    { 0.021066f, -0.037252f, -0.092041f}, { 0.014681f, -0.043044f, -0.057739f},
    {-0.008493f, -0.008143f, -0.102486f}, {-0.002303f, -0.061540f, -0.022952f},
    {-0.006061f, -0.014278f, -0.033652f}, {-0.005276f,  0.011246f, -0.062762f},
    { 0.043411f, -0.006303f, -0.063730f}, { 0.035885f, -0.010052f, -0.115290f},
    { 0.030628f, -0.031538f, -0.017807f}, { 0.022345f,  0.028210f, -0.032335f},
    { 0.026535f,  0.027536f, -0.091150f}, {-0.003365f, -0.008077f,  0.015687f},
    {-0.026013f,  0.017493f, -0.010355f}, { 0.059069f,  0.010634f, -0.007530f},
    { 0.044038f, -0.019424f,  0.030453f}, {-0.036065f, -0.034215f, -0.007758f},
    { 0.022486f,  0.042543f,  0.027870f}, {-0.049985f, -0.016085f,  0.021768f},
    {-0.021715f,  0.021168f,  0.052076f}, {-0.004243f, -0.061228f,  0.027640f},
    {-0.033950f, -0.017287f,  0.064656f}, { 0.016151f,  0.000727f,  0.062757f},
    {-0.063456f, -0.043152f,  0.056707f}, {-0.067715f,  0.006126f,  0.058178f},
    {-0.038931f,  0.051673f,  0.030636f}, {-0.073017f, -0.074716f,  0.026387f},
    {-0.039893f, -0.104629f,  0.039616f}, {-0.073179f, -0.074601f,  0.082069f},
    {-0.066154f, -0.027180f,  0.099439f}, {-0.075167f, -0.121149f,  0.071938f},
    {-0.030382f, -0.092582f,  0.091067f}, {-0.084519f, -0.137542f,  0.023626f},
    {-0.060956f, -0.121259f, -0.015264f}, {-0.030069f, -0.093823f, -0.008692f},
    {-0.063564f, -0.065225f, -0.025820f}, {-0.052074f, -0.117595f, -0.059689f},
    {-0.091652f, -0.165173f, -0.045573f}, {-0.070167f, -0.121342f,  0.131707f},
    {-0.061024f, -0.005833f, -0.051035f}, { 0.007837f, -0.051816f,  0.074575f},
    {-0.070643f, -0.053927f,  0.149498f}, {-0.014358f, -0.066681f,  0.139708f},
    {-0.058186f,  0.029576f,  0.092923f}, {-0.023371f,  0.007058f,  0.112484f},
    {-0.057969f,  0.022786f,  0.148420f}, { 0.029439f, -0.017673f,  0.121423f},
    {-0.015811f,  0.056785f,  0.091594f}, { 0.004347f,  0.056680f,  0.137848f},
    {-0.004464f,  0.002342f,  0.184013f}, { 0.029660f,  0.046870f,  0.082654f},
    { 0.059408f,  0.001016f,  0.086063f}, { 0.055263f,  0.027574f,  0.155037f},
    { 0.062166f,  0.064323f,  0.117371f}, { 0.022967f,  0.100050f,  0.077227f},
    { 0.041795f,  0.096343f,  0.170421f}, { 0.053189f,  0.122931f,  0.118549f},
    { 0.094247f,  0.094448f,  0.078395f}, { 0.082407f,  0.033408f,  0.041085f},
    { 0.096820f,  0.115960f,  0.149433f}, { 0.067804f,  0.121849f,  0.025336f},
    {-0.008421f,  0.104316f,  0.032314f}, { 0.031013f,  0.073218f, -0.004899f},
    { 0.085079f,  0.060323f, -0.009687f}, { 0.028174f,  0.092766f, -0.055590f},
    { 0.070133f,  0.039160f, -0.061035f}, {-0.039211f,  0.072517f, -0.028756f},
    { 0.129686f,  0.100233f, -0.046998f}, { 0.154189f,  0.107616f,  0.022791f},
    {-0.049331f,  0.094184f,  0.087984f}, {-0.013179f,  0.126552f,  0.125099f},
    {-0.058716f,  0.098524f,  0.150886f}, {-0.022753f,  0.080011f,  0.191127f},
    { 0.013451f,  0.164593f,  0.153768f}, { 0.074818f,  0.181214f,  0.108211f},
    { 0.091323f,  0.169249f,  0.168460f}, { 0.033885f,  0.155516f,  0.213745f},
    {-0.032128f,  0.227238f,  0.135815f}, {-0.059176f,  0.168980f,  0.229110f},
    { 0.033917f,  0.229753f,  0.222264f}, { 0.082702f,  0.116370f,  0.224697f},
    { 0.127737f,  0.186658f,  0.212783f}, { 0.047528f,  0.063920f,  0.216856f},
    {-0.002446f,  0.114174f,  0.263289f}, {-0.077783f,  0.082523f,  0.249697f},
    { 0.010023f,  0.024267f,  0.256874f}, { 0.053190f,  0.111422f,  0.310407f},
    {-0.078804f,  0.004444f,  0.224078f}, {-0.055253f, -0.059180f,  0.217892f},
    {-0.065371f,  0.008124f,  0.333405f}, {-0.076188f, -0.098767f,  0.286983f},
    {-0.071911f, -0.115804f,  0.198031f}, {-0.062473f,  0.183639f,  0.370162f},
    {-0.042666f,  0.255210f,  0.262720f}, { 0.011999f,  0.217530f,  0.318291f},
    {-0.042144f,  0.322087f,  0.326387f}, { 0.090663f,  0.205835f,  0.294784f},
    { 0.058218f,  0.293649f,  0.277927f}, { 0.157506f,  0.282870f,  0.294610f},
    { 0.118248f,  0.261007f,  0.148614f}, { 0.065261f,  0.332362f,  0.411912f},
    { 0.141269f,  0.451850f,  0.315726f}, { 0.001706f,  0.456301f,  0.357590f},
    {-0.052947f,  0.356559f,  0.456944f}, { 0.247707f,  0.263837f,  0.152591f},
    { 0.306847f,  0.417373f,  0.258553f}, { 0.166347f,  0.149222f,  0.118973f},
    { 0.379709f,  0.292172f,  0.139875f}, { 0.010171f, -0.055170f, -0.174523f}
};

static const float lsf_cb2_16k[256][3] = {
    {-0.213011f, -0.293385f, -0.330597f}, {-0.212582f, -0.240992f, -0.338239f},
    {-0.223373f, -0.306214f, -0.277192f}, {-0.231138f, -0.287729f, -0.229412f},
    {-0.238466f, -0.228571f, -0.260954f}, {-0.140931f, -0.247018f, -0.258566f},
    {-0.136239f, -0.249669f, -0.350143f}, {-0.149738f, -0.192970f, -0.281475f},
    {-0.167058f, -0.261052f, -0.196301f}, {-0.177049f, -0.201324f, -0.207897f},
    {-0.116915f, -0.200629f, -0.212526f}, {-0.162247f, -0.143805f, -0.245093f},
    {-0.082042f, -0.191842f, -0.266338f}, {-0.098846f, -0.208511f, -0.320481f},
    {-0.113510f, -0.152470f, -0.222474f}, {-0.066197f, -0.179112f, -0.207813f},
    {-0.129490f, -0.169320f, -0.155238f}, {-0.078843f, -0.190292f, -0.155172f},
    {-0.087790f, -0.147729f, -0.169351f}, {-0.141037f, -0.127207f, -0.177910f},
    {-0.126525f, -0.223961f, -0.153639f}, {-0.101464f, -0.189953f, -0.114559f},
    {-0.102450f, -0.106303f, -0.151171f}, {-0.103208f, -0.144457f, -0.105378f},
    {-0.170794f, -0.140525f, -0.136428f}, {-0.168641f, -0.203064f, -0.135368f},
    {-0.138193f, -0.116042f, -0.111905f}, {-0.145085f, -0.168581f, -0.092613f},
    {-0.126379f, -0.220431f, -0.091327f}, {-0.212934f, -0.184797f, -0.101632f},
    {-0.193711f, -0.140556f, -0.078304f}, {-0.173674f, -0.197276f, -0.060140f},
    {-0.197897f, -0.241907f, -0.091997f}, {-0.156037f, -0.258519f, -0.111628f},
    {-0.241964f, -0.191124f, -0.063140f}, {-0.261340f, -0.240847f, -0.103132f},
    {-0.221621f, -0.242972f, -0.041255f}, {-0.224166f, -0.232742f, -0.161568f},
    {-0.203591f, -0.294470f, -0.126035f}, {-0.209540f, -0.303149f, -0.053170f},
    {-0.253859f, -0.295066f, -0.156050f}, {-0.278143f, -0.331105f, -0.085237f},
    {-0.300273f, -0.198750f, -0.094834f}, {-0.260477f, -0.169713f, -0.132476f},
    {-0.211889f, -0.172454f, -0.164281f}, {-0.228370f, -0.122149f, -0.124178f},
    {-0.254629f, -0.135668f, -0.081692f}, {-0.263813f, -0.154928f, -0.213596f},
    {-0.308224f, -0.106877f, -0.084404f}, {-0.242644f, -0.082862f, -0.085835f},
    {-0.252084f, -0.064888f, -0.146498f}, {-0.198162f, -0.105721f, -0.188887f},
    {-0.189238f, -0.088028f, -0.109736f}, {-0.197598f, -0.099831f, -0.044030f},
    {-0.269017f, -0.105991f, -0.021513f}, {-0.231349f, -0.058825f, -0.041407f},
    {-0.225589f, -0.027501f, -0.087160f}, {-0.160347f, -0.058341f, -0.079789f},
    {-0.158729f, -0.108951f, -0.067262f}, {-0.170483f, -0.053023f, -0.017561f},
    {-0.175207f, -0.013649f, -0.049513f}, {-0.156004f, -0.108378f, -0.004052f},
    {-0.219958f, -0.082362f,  0.014950f}, {-0.217785f, -0.012981f, -0.009410f},
    {-0.123290f, -0.040849f, -0.040910f}, {-0.119861f, -0.095078f, -0.060246f},
    {-0.117537f, -0.065479f,  0.002968f}, {-0.103231f, -0.113298f, -0.023282f},
    {-0.136365f, -0.149524f, -0.051387f}, {-0.119332f, -0.164400f, -0.009103f},
    {-0.104522f, -0.060948f, -0.083056f}, {-0.071461f, -0.070787f, -0.037347f},
    {-0.081116f, -0.149015f, -0.056740f}, {-0.069561f, -0.108099f, -0.069167f},
    {-0.055624f, -0.117369f, -0.025091f}, {-0.091941f, -0.190091f, -0.060020f},
    {-0.072003f, -0.168433f, -0.006540f}, {-0.033305f, -0.154427f, -0.054608f},
    {-0.062988f, -0.127093f, -0.108307f}, {-0.056690f, -0.170813f, -0.102834f},
    {-0.018273f, -0.127863f, -0.094998f}, {-0.056239f, -0.123678f, -0.146262f},
    {-0.023442f, -0.154617f, -0.137417f}, {-0.051903f, -0.078379f, -0.093395f},
    {-0.014599f, -0.104412f, -0.135959f}, {-0.051582f, -0.081280f, -0.140643f},
    {-0.092727f, -0.091930f, -0.107816f}, {-0.024814f, -0.140993f, -0.183243f},
    {-0.064307f, -0.113024f, -0.194788f}, {-0.000118f, -0.098858f, -0.195336f},
    {-0.028090f, -0.048258f, -0.164101f}, {-0.093414f, -0.055969f, -0.172743f},
    {-0.114445f, -0.104336f, -0.215204f}, {-0.048518f, -0.132063f, -0.242991f},
    {-0.159620f, -0.060240f, -0.178592f}, {-0.135728f, -0.067473f, -0.131876f},
    {-0.078464f, -0.038040f, -0.125105f}, {-0.011061f, -0.064011f, -0.102496f},
    {-0.033887f, -0.026485f, -0.109493f}, {-0.129128f, -0.014216f, -0.111329f},
    {-0.190687f, -0.030660f, -0.135825f}, {-0.082037f,  0.010997f, -0.100167f},
    {-0.183403f,  0.001651f, -0.098962f}, {-0.074775f, -0.030335f, -0.062217f},
    {-0.031759f, -0.050551f, -0.059420f}, {-0.051439f,  0.010827f, -0.052148f},
    {-0.126744f,  0.008689f, -0.047785f}, {-0.145916f,  0.042019f, -0.077431f},
    {-0.093552f,  0.054143f, -0.060473f}, {-0.090660f,  0.012868f, -0.018195f},
    {-0.079783f, -0.033071f,  0.001482f}, {-0.033010f, -0.022331f, -0.014506f},
    {-0.004798f, -0.017339f, -0.060120f}, {-0.025021f,  0.026390f, -0.003263f},
    {-0.001437f,  0.025994f, -0.040892f}, {-0.074821f,  0.019005f,  0.027549f},
    {-0.030811f, -0.012114f,  0.034284f}, { 0.006785f,  0.004618f,  0.018717f},
    { 0.013392f, -0.032597f, -0.023731f}, { 0.035994f,  0.005963f, -0.011757f},
    { 0.008071f, -0.045750f,  0.024889f}, { 0.013055f,  0.017040f,  0.054121f},
    {-0.012989f,  0.044864f,  0.036327f}, { 0.025054f,  0.047137f,  0.009974f},
    { 0.053801f,  0.024178f,  0.031774f}, { 0.056442f, -0.030647f,  0.021291f},
    { 0.032247f,  0.052680f,  0.049886f}, { 0.035369f,  0.090207f,  0.031394f},
    { 0.064720f,  0.070390f,  0.040938f}, { 0.022112f,  0.054834f,  0.091059f},
    { 0.041765f,  0.086248f,  0.070196f}, { 0.070645f,  0.060852f,  0.078825f},
    { 0.058506f,  0.016920f,  0.081612f}, { 0.000009f,  0.086500f,  0.059849f},
    { 0.071253f,  0.107392f,  0.059046f}, { 0.094702f,  0.096160f,  0.090982f},
    { 0.047639f,  0.110877f,  0.111227f}, { 0.122444f,  0.090909f,  0.057396f},
    { 0.101916f,  0.052299f,  0.029909f}, { 0.076560f,  0.086094f, -0.007252f},
    { 0.123411f,  0.030769f,  0.082749f}, { 0.135579f,  0.103022f,  0.009540f},
    { 0.120576f,  0.065284f, -0.024095f}, { 0.077483f,  0.028526f, -0.012369f},
    { 0.128747f,  0.017901f, -0.003874f}, { 0.158254f,  0.046962f,  0.029577f},
    { 0.102287f, -0.002211f,  0.037329f}, { 0.089654f, -0.021372f, -0.006857f},
    { 0.137917f,  0.027228f, -0.053223f}, { 0.098728f, -0.012192f, -0.048518f},
    { 0.083974f,  0.036153f, -0.062266f}, { 0.048230f, -0.010241f, -0.052293f},
    { 0.110135f,  0.007715f, -0.095233f}, { 0.068294f, -0.014317f, -0.104029f},
    { 0.063909f, -0.056416f, -0.063023f}, { 0.059133f, -0.044675f, -0.023780f},
    { 0.030748f,  0.021845f, -0.086332f}, { 0.023994f, -0.045574f, -0.076232f},
    { 0.052147f, -0.059825f, -0.109667f}, { 0.013087f, -0.020420f, -0.121945f},
    { 0.018163f, -0.096765f, -0.088758f}, { 0.020196f, -0.076470f, -0.048112f},
    { 0.020282f, -0.084204f, -0.135535f}, { 0.040076f, -0.053464f, -0.161949f},
    {-0.017796f, -0.103070f, -0.059559f}, {-0.016484f, -0.070138f, -0.016866f},
    { 0.004849f, -0.112481f, -0.017731f}, { 0.040160f, -0.073873f, -0.005327f},
    { 0.002202f, -0.094723f,  0.045366f}, {-0.056918f, -0.081578f,  0.017875f},
    {-0.031099f, -0.141708f,  0.009186f}, {-0.102802f, -0.122675f,  0.030060f},
    {-0.061717f, -0.145116f,  0.076680f}, {-0.073607f, -0.050464f,  0.072853f},
    {-0.117403f, -0.194921f,  0.040101f}, {-0.185236f, -0.133620f,  0.045939f},
    {-0.160174f, -0.057226f,  0.056641f}, {-0.178489f, -0.173435f, -0.007806f},
    {-0.199916f, -0.204866f,  0.047342f}, {-0.152337f, -0.249651f,  0.034656f},
    {-0.185637f, -0.230942f, -0.002072f}, {-0.122548f, -0.215209f, -0.024552f},
    {-0.249578f, -0.209714f,  0.009470f}, {-0.160108f, -0.257702f, -0.040992f},
    {-0.216694f, -0.289353f,  0.027182f}, {-0.226390f, -0.147844f, -0.022742f},
    {-0.288737f, -0.272150f, -0.013948f}, {-0.262554f, -0.237035f,  0.072473f},
    {-0.306267f, -0.188335f, -0.032894f}, {-0.259666f, -0.345816f,  0.024138f},
    {-0.271093f, -0.137143f,  0.040404f}, {-0.201317f, -0.286782f,  0.107615f},
    {-0.235725f, -0.163396f,  0.113844f}, {-0.159988f, -0.209788f,  0.112140f},
    {-0.262985f, -0.056741f,  0.093506f}, {-0.277226f, -0.037306f,  0.016008f},
    {-0.293486f, -0.040422f, -0.062018f}, {-0.214921f,  0.022900f,  0.055295f},
    {-0.253889f,  0.058575f, -0.000151f}, {-0.246689f,  0.024242f, -0.058488f},
    {-0.143790f,  0.006767f,  0.014061f}, {-0.187077f,  0.048882f, -0.035625f},
    {-0.196369f,  0.112085f,  0.031546f}, {-0.124264f,  0.086197f, -0.020800f},
    {-0.126249f,  0.016960f,  0.095741f}, {-0.079816f,  0.080398f,  0.051038f},
    {-0.056269f,  0.075380f, -0.028262f}, {-0.120493f,  0.148495f,  0.028430f},
    {-0.161750f,  0.101290f,  0.117806f}, {-0.003247f,  0.083393f, -0.017061f},
    {-0.034007f,  0.142542f,  0.007402f}, {-0.037618f,  0.025871f,  0.089496f},
    {-0.082819f,  0.184435f,  0.073224f}, { 0.006448f,  0.167015f,  0.080548f},
    { 0.035315f,  0.144022f,  0.003218f}, {-0.023459f,  0.088147f,  0.152604f},
    { 0.006247f, -0.024099f,  0.077792f}, { 0.039894f,  0.057586f, -0.042455f},
    {-0.020417f,  0.035400f, -0.093971f}, { 0.075465f,  0.052063f,  0.145582f},
    { 0.078027f,  0.184720f,  0.092096f}, { 0.107295f,  0.148380f,  0.022264f},
    { 0.066928f, -0.052831f,  0.065108f}, { 0.093295f,  0.118157f,  0.149815f},
    { 0.119373f,  0.137114f,  0.099536f}, { 0.138653f,  0.075509f,  0.121545f},
    { 0.174025f,  0.077531f,  0.077169f}, { 0.165839f,  0.150080f,  0.133423f},
    { 0.173276f,  0.155887f,  0.048150f}, { 0.162910f,  0.095898f,  0.171896f},
    { 0.214577f,  0.112888f,  0.115579f}, { 0.204755f,  0.106392f,  0.032337f},
    { 0.178853f,  0.205034f,  0.114760f}, { 0.177401f,  0.070504f, -0.013778f},
    { 0.241624f,  0.166921f,  0.066087f}, { 0.219595f,  0.183553f,  0.172332f},
    { 0.123671f,  0.170842f,  0.167216f}, { 0.177104f,  0.240197f,  0.186359f},
    { 0.272003f,  0.220214f,  0.126073f}, { 0.093748f,  0.235843f,  0.160998f},
    { 0.141510f,  0.190012f,  0.240416f}, { 0.046878f,  0.168984f,  0.190412f},
    { 0.094898f,  0.107038f,  0.235003f}, { 0.108592f,  0.269536f,  0.262528f},
    {-0.027754f,  0.234355f,  0.134544f}, { 0.265127f,  0.267540f,  0.199041f},
    { 0.199523f,  0.291507f,  0.265171f}, { 0.266177f,  0.209339f,  0.350369f},
    { 0.322159f,  0.344794f,  0.270823f}, { 0.399957f,  0.264065f,  0.110387f},
    { 0.277817f,  0.127407f, -0.035625f}, {-0.177038f,  0.208155f,  0.119077f},
    { 0.049075f, -0.076294f,  0.145711f}, { 0.187246f,  0.042865f, -0.127097f},
    { 0.117885f, -0.023489f, -0.138658f}, {-0.284256f,  0.068153f,  0.124259f}
};

static const float lsf_cb3_16k[128][3] = {
    {-0.223412f, -0.236300f, -0.188067f}, {-0.202286f, -0.218711f, -0.102947f},
    {-0.251652f, -0.161020f, -0.125280f}, {-0.169223f, -0.138155f, -0.140430f},
    {-0.176427f, -0.146628f, -0.222632f}, {-0.120584f, -0.187276f, -0.180164f},
    {-0.195559f, -0.074225f, -0.169109f}, {-0.144551f, -0.142774f, -0.073340f},
    {-0.111001f, -0.111310f, -0.130696f}, {-0.095221f, -0.174684f, -0.111841f},
    {-0.112158f, -0.103049f, -0.195130f}, {-0.059989f, -0.142170f, -0.157850f},
    {-0.127598f, -0.051759f, -0.153109f}, {-0.063753f, -0.067898f, -0.164117f},
    {-0.141753f, -0.068274f, -0.091999f}, {-0.060482f, -0.101054f, -0.099475f},
    {-0.104699f, -0.104456f, -0.066496f}, {-0.073649f, -0.052614f, -0.091612f},
    {-0.088268f, -0.019072f, -0.129956f}, {-0.018837f, -0.104115f, -0.127837f},
    {-0.021630f, -0.033055f, -0.129868f}, {-0.083768f, -0.047549f, -0.041407f},
    {-0.055892f, -0.108526f, -0.043200f}, {-0.027816f, -0.062499f, -0.048190f},
    {-0.002248f, -0.110428f, -0.062868f}, { 0.001270f, -0.033245f, -0.072404f},
    {-0.042747f, -0.013835f, -0.033829f}, {-0.037615f, -0.147833f, -0.083912f},
    {-0.045023f,  0.006011f, -0.092182f}, {-0.050411f, -0.081832f,  0.005787f},
    { 0.000357f, -0.104282f, -0.009428f}, {-0.003893f, -0.047892f, -0.001506f},
    {-0.040077f, -0.147110f, -0.009065f}, {-0.060858f, -0.030972f,  0.012999f},
    {-0.014674f,  0.001370f,  0.005554f}, {-0.101362f, -0.126061f, -0.001898f},
    {-0.102519f, -0.000390f, -0.015721f}, {-0.132687f, -0.069608f, -0.019928f},
    {-0.102227f, -0.076131f,  0.043306f}, {-0.055193f,  0.027001f,  0.011857f},
    {-0.156427f, -0.016629f,  0.017480f}, {-0.078736f,  0.002809f,  0.057979f},
    {-0.157789f, -0.016693f, -0.055073f}, {-0.179397f, -0.095520f,  0.022065f},
    {-0.110219f,  0.010408f, -0.081927f}, {-0.125392f,  0.049111f,  0.044595f},
    {-0.112528f,  0.063173f, -0.024954f}, {-0.185525f,  0.053093f, -0.032102f},
    {-0.176887f, -0.019379f, -0.115125f}, {-0.249706f, -0.017664f, -0.059188f},
    {-0.200243f, -0.103311f, -0.066846f}, {-0.055404f,  0.045106f, -0.046991f},
    {-0.000544f,  0.022690f, -0.044831f}, { 0.022298f, -0.016367f, -0.022509f},
    { 0.028278f,  0.017585f, -0.100612f}, { 0.061781f, -0.020826f, -0.068190f},
    { 0.029157f, -0.074477f, -0.098898f}, { 0.043073f, -0.067234f, -0.032293f},
    { 0.060157f,  0.034636f, -0.034885f}, { 0.071153f, -0.013881f, -0.009036f},
    { 0.054196f, -0.029989f, -0.131139f}, { 0.030193f,  0.024976f,  0.009861f},
    { 0.055943f, -0.045304f,  0.031927f}, { 0.033217f, -0.002418f,  0.038165f},
    { 0.063814f,  0.045625f,  0.025309f}, { 0.033689f,  0.038819f,  0.049700f},
    { 0.073582f,  0.028527f,  0.060200f}, {-0.007957f,  0.022531f,  0.043687f},
    {-0.000984f,  0.054518f,  0.018742f}, { 0.057004f,  0.060916f,  0.060573f},
    { 0.009883f,  0.015238f,  0.080211f}, { 0.022742f,  0.070832f,  0.068855f},
    { 0.053001f,  0.029790f,  0.091446f}, {-0.042447f,  0.060379f,  0.061462f},
    { 0.076826f,  0.062468f,  0.089653f}, { 0.039065f,  0.069768f,  0.119128f},
    { 0.064145f,  0.095353f,  0.071621f}, { 0.094411f,  0.069527f,  0.054197f},
    { 0.042812f,  0.093060f,  0.027980f}, { 0.094791f,  0.099189f,  0.101112f},
    { 0.117611f,  0.048601f,  0.093111f}, { 0.119951f,  0.122758f,  0.051546f},
    { 0.103558f,  0.085245f, -0.010700f}, { 0.150126f,  0.059766f,  0.020280f},
    { 0.108066f,  0.017170f,  0.008606f}, { 0.108422f,  0.023253f, -0.063942f},
    { 0.019652f,  0.072284f, -0.030331f}, { 0.192719f,  0.075624f,  0.071156f},
    { 0.221140f,  0.069191f, -0.035085f}, { 0.188367f,  0.126200f,  0.035225f},
    { 0.185760f,  0.043537f, -0.101714f}, {-0.042518f,  0.099646f,  0.003244f},
    {-0.015308f, -0.027521f,  0.046006f}, { 0.034086f, -0.045777f,  0.095989f},
    { 0.007174f, -0.093358f,  0.046459f}, {-0.051248f, -0.062095f,  0.083161f},
    {-0.045626f, -0.133301f,  0.052997f}, {-0.037840f,  0.024042f,  0.131097f},
    {-0.020217f, -0.115942f,  0.126170f}, {-0.134550f, -0.036291f,  0.111322f},
    {-0.110576f, -0.160024f,  0.091841f}, {-0.093308f, -0.184958f,  0.013939f},
    {-0.082735f, -0.167417f, -0.051725f}, {-0.169934f, -0.173003f, -0.007155f},
    {-0.128244f, -0.213123f, -0.053337f}, {-0.079852f, -0.154116f, -0.246546f},
    {-0.032242f, -0.108756f, -0.204133f}, {-0.140117f, -0.199495f, -0.284505f},
    { 0.010842f, -0.074979f, -0.166333f}, {-0.093313f,  0.145006f,  0.034110f},
    {-0.039236f,  0.113213f,  0.111053f}, { 0.040613f, -0.031783f,  0.174058f},
    {-0.164232f,  0.131421f,  0.149842f}, { 0.026893f,  0.107281f,  0.179297f},
    { 0.047086f,  0.158606f,  0.103267f}, {-0.070567f,  0.210459f,  0.134734f},
    { 0.094392f,  0.137050f,  0.166892f}, { 0.086039f,  0.063657f,  0.168825f},
    { 0.159371f,  0.120897f,  0.154357f}, { 0.147101f,  0.160684f,  0.114882f},
    { 0.120158f,  0.199650f,  0.180948f}, { 0.191417f,  0.174500f,  0.170734f},
    { 0.159153f,  0.142165f,  0.233347f}, { 0.232002f,  0.150181f,  0.102736f},
    { 0.188299f,  0.221738f,  0.228748f}, { 0.256786f,  0.209685f,  0.161534f},
    { 0.257861f,  0.247793f,  0.250516f}, {-0.164461f, -0.000143f,  0.232461f}
};

static const float lsf_cb4_16k[128][3] = {
    {-0.193369f, -0.304643f, -0.253777f}, {-0.164125f, -0.277786f, -0.153116f},
    {-0.135681f, -0.209120f, -0.211724f}, {-0.121822f, -0.215734f, -0.292207f},
    {-0.198781f, -0.161674f, -0.242538f}, {-0.164147f, -0.180570f, -0.138070f},
    {-0.095915f, -0.198695f, -0.154309f}, {-0.248386f, -0.234462f, -0.136984f},
    {-0.164968f, -0.108318f, -0.175635f}, {-0.124171f, -0.111809f, -0.224402f},
    {-0.067398f, -0.157017f, -0.195759f}, {-0.090132f, -0.119174f, -0.165253f},
    {-0.099460f, -0.146895f, -0.106799f}, {-0.141493f, -0.108103f, -0.108880f},
    {-0.085088f, -0.098340f, -0.109953f}, {-0.105526f, -0.054463f, -0.154315f},
    {-0.040480f, -0.144285f, -0.124042f}, {-0.040969f, -0.084039f, -0.142880f},
    {-0.049082f, -0.118553f, -0.066686f}, {-0.096336f, -0.087515f, -0.055741f},
    {-0.058605f, -0.059327f, -0.089275f}, {-0.121842f, -0.058681f, -0.086949f},
    {-0.053792f, -0.022025f, -0.124451f}, {-0.036744f, -0.068891f, -0.045865f},
    { 0.003900f, -0.098237f, -0.091158f}, {-0.001664f, -0.045089f, -0.081353f},
    {-0.072829f, -0.034087f, -0.038416f}, {-0.100822f, -0.007330f, -0.088715f},
    {-0.035911f, -0.005864f, -0.062577f}, {-0.020205f, -0.026547f, -0.019634f},
    { 0.004291f, -0.041290f, -0.138181f}, { 0.023404f, -0.010932f, -0.044904f},
    { 0.013557f,  0.014823f, -0.092943f}, { 0.059673f, -0.031024f, -0.095739f},
    { 0.021130f, -0.080607f, -0.034594f}, { 0.024655f, -0.035564f,  0.003243f},
    { 0.017106f,  0.006952f, -0.000308f}, { 0.075208f, -0.030910f, -0.031181f},
    { 0.024965f,  0.048632f, -0.039448f}, { 0.057028f,  0.021547f, -0.009418f},
    {-0.018577f,  0.023697f, -0.009759f}, { 0.024077f,  0.033053f,  0.024324f},
    { 0.037052f, -0.003436f,  0.044530f}, {-0.012871f, -0.007179f,  0.031795f},
    { 0.077877f,  0.021547f,  0.023131f}, { 0.053365f,  0.052078f,  0.029433f},
    { 0.011429f,  0.070426f,  0.028734f}, {-0.001827f,  0.033115f,  0.061505f},
    {-0.044870f,  0.038568f,  0.026239f}, { 0.061633f,  0.034799f,  0.059784f},
    { 0.034261f,  0.060342f,  0.065185f}, { 0.058981f,  0.082481f,  0.047252f},
    { 0.090008f,  0.065942f,  0.044470f}, { 0.066961f,  0.073728f, -0.000428f},
    { 0.074763f,  0.060293f,  0.085632f}, { 0.066366f,  0.103375f,  0.079642f},
    { 0.122297f,  0.036558f,  0.058745f}, { 0.111042f,  0.092093f,  0.085412f},
    { 0.099243f,  0.115476f,  0.039254f}, { 0.019973f,  0.122844f,  0.050255f},
    { 0.159571f,  0.098965f,  0.051740f}, { 0.137624f,  0.072405f, -0.006922f},
    { 0.130240f,  0.146091f,  0.089698f}, { 0.138335f,  0.092968f,  0.136193f},
    { 0.066031f,  0.149304f,  0.125476f}, { 0.202749f,  0.145751f,  0.077122f},
    { 0.002224f,  0.082811f,  0.131200f}, { 0.124476f,  0.178073f,  0.162336f},
    { 0.174722f,  0.190298f,  0.127106f}, { 0.202193f,  0.153569f,  0.163840f},
    { 0.242604f,  0.197796f,  0.136929f}, { 0.185809f,  0.229348f,  0.193353f},
    {-0.058814f,  0.195178f,  0.141821f}, { 0.253646f,  0.247175f,  0.205766f},
    { 0.061433f, -0.025542f,  0.119311f}, {-0.057816f,  0.082445f,  0.073243f},
    {-0.069239f,  0.148678f,  0.031146f}, {-0.030217f, -0.008503f,  0.106194f},
    {-0.026708f,  0.087469f, -0.009589f}, {-0.090418f,  0.000265f,  0.056807f},
    {-0.050607f, -0.019383f,  0.010494f}, {-0.079397f,  0.008233f, -0.011469f},
    {-0.072634f, -0.061165f,  0.046917f}, {-0.075741f, -0.072343f, -0.007557f},
    {-0.025162f, -0.073363f,  0.005173f}, {-0.123371f, -0.041257f, -0.008375f},
    {-0.139904f,  0.018285f,  0.009920f}, {-0.143421f, -0.104238f,  0.033457f},
    {-0.100923f, -0.134400f, -0.023257f}, {-0.157791f, -0.095042f, -0.036959f},
    {-0.219890f, -0.078637f,  0.001815f}, {-0.183607f, -0.023053f, -0.043678f},
    {-0.145303f, -0.158923f, -0.059045f}, {-0.197615f, -0.165199f,  0.028099f},
    {-0.225131f, -0.167756f, -0.056401f}, {-0.216572f, -0.104751f, -0.102964f},
    {-0.171336f, -0.241967f, -0.063404f}, {-0.134035f, -0.205614f,  0.011831f},
    {-0.297116f, -0.211173f, -0.015352f}, {-0.086464f, -0.200592f, -0.070454f},
    {-0.217777f, -0.278403f,  0.030398f}, {-0.236248f, -0.323694f, -0.087588f},
    {-0.222074f, -0.210785f,  0.106210f}, {-0.283400f, -0.097077f,  0.041303f},
    {-0.078417f, -0.154464f,  0.062956f}, {-0.214417f, -0.100695f,  0.121909f},
    {-0.178576f, -0.028847f,  0.061042f}, {-0.037999f, -0.144233f, -0.010546f},
    {-0.086695f, -0.070996f,  0.125282f}, { 0.010788f, -0.085006f,  0.058527f},
    {-0.154015f,  0.066560f,  0.071038f}, {-0.143503f,  0.033260f,  0.154393f},
    {-0.134069f,  0.032420f, -0.056293f}, {-0.110851f,  0.086908f,  0.003920f},
    {-0.057254f,  0.047674f, -0.055571f}, {-0.214206f,  0.068784f, -0.004735f},
    {-0.257264f,  0.050468f,  0.081702f}, {-0.291834f,  0.004120f, -0.022366f},
    {-0.173309f, -0.029081f, -0.115901f}, {-0.207622f,  0.168664f,  0.136030f},
    { 0.090541f,  0.032754f, -0.057330f}, { 0.140219f, -0.000735f, -0.015633f},
    { 0.136697f, -0.017163f, -0.100909f}, { 0.029838f, -0.089515f, -0.147130f},
    {-0.055367f, -0.072683f, -0.214015f}, { 0.048680f, -0.057633f, -0.212429f},
    {-0.013134f, -0.113898f, -0.196403f}, {-0.071702f, -0.159408f, -0.254895f}
};

static const float lsf_cb5_16k[128][4] = {
    {-0.201277f, -0.278679f, -0.173262f, -0.198580f},
    {-0.214667f, -0.151922f, -0.117551f, -0.192713f},
    {-0.160962f, -0.207728f, -0.124750f, -0.129749f},
    {-0.131043f, -0.137818f, -0.155281f, -0.166308f},
    {-0.179134f, -0.169602f, -0.165223f, -0.066293f},
    {-0.136474f, -0.177035f, -0.250127f, -0.134370f},
    {-0.066970f, -0.146274f, -0.170638f, -0.134436f},
    {-0.083288f, -0.165860f, -0.103437f, -0.140361f},
    {-0.130474f, -0.119317f, -0.124393f, -0.086408f},
    {-0.127609f, -0.134415f, -0.073592f, -0.116103f},
    {-0.113027f, -0.091756f, -0.107786f, -0.131935f},
    {-0.125530f, -0.182152f, -0.093796f, -0.045088f},
    {-0.077122f, -0.138052f, -0.166271f, -0.038886f},
    {-0.073027f, -0.106845f, -0.067073f, -0.113910f},
    {-0.049146f, -0.107019f, -0.112531f, -0.063388f},
    {-0.101539f, -0.119586f, -0.050297f, -0.040670f},
    {-0.107784f, -0.066913f, -0.080993f, -0.052352f},
    {-0.152155f, -0.103010f, -0.090461f, -0.015526f},
    {-0.153087f, -0.087656f, -0.029889f, -0.037367f},
    {-0.215281f, -0.138062f, -0.089162f, -0.050839f},
    {-0.053350f, -0.060169f, -0.063459f, -0.024499f},
    {-0.051674f, -0.076355f, -0.033733f, -0.077211f},
    {-0.045047f, -0.107006f, -0.020880f, -0.024525f},
    {-0.083003f, -0.063672f, -0.013243f, -0.028324f},
    {-0.104104f, -0.075450f, -0.032746f,  0.024480f},
    {-0.085695f, -0.019502f, -0.045121f, -0.025016f},
    {-0.123120f, -0.030844f, -0.003533f, -0.016224f},
    {-0.025568f, -0.049172f, -0.003911f, -0.027522f},
    {-0.039029f, -0.019857f, -0.043211f, -0.058087f},
    {-0.040122f, -0.023067f, -0.001356f,  0.008607f},
    {-0.063351f, -0.001776f,  0.016015f, -0.027088f},
    {-0.068110f, -0.038838f,  0.042525f,  0.001076f},
    {-0.043623f, -0.020736f, -0.047862f,  0.037710f},
    {-0.041052f,  0.021954f, -0.025660f,  0.000758f},
    {-0.013035f,  0.002583f, -0.008233f, -0.037300f},
    {-0.005523f, -0.014670f,  0.019651f, -0.012667f},
    {-0.004409f, -0.014437f, -0.059412f, -0.019701f},
    { 0.024946f, -0.011663f, -0.014351f, -0.028762f},
    { 0.012660f,  0.018489f, -0.010205f,  0.012695f},
    {-0.004423f,  0.017827f,  0.040544f,  0.003629f},
    { 0.020684f,  0.026743f,  0.007752f, -0.025595f},
    { 0.032071f,  0.000043f,  0.026188f, -0.006444f},
    { 0.058793f,  0.015820f, -0.001119f, -0.017415f},
    { 0.020156f, -0.047590f,  0.004227f,  0.008670f},
    { 0.054770f,  0.032135f,  0.029770f, -0.009767f},
    { 0.030884f,  0.047757f,  0.033068f,  0.006866f},
    { 0.062039f,  0.011646f,  0.056037f,  0.016859f},
    { 0.013798f, -0.028196f,  0.060710f,  0.014299f},
    { 0.100043f,  0.041445f,  0.023379f, -0.014889f},
    { 0.062728f, -0.042821f,  0.002180f, -0.055380f},
    { 0.061663f,  0.018767f, -0.015571f, -0.074095f},
    { 0.062980f,  0.080497f,  0.011808f, -0.031787f},
    { 0.084964f,  0.043100f, -0.025877f,  0.020309f},
    { 0.014707f,  0.035421f, -0.041440f, -0.053373f},
    { 0.081268f,  0.005791f, -0.066290f, -0.039825f},
    { 0.017691f, -0.020401f, -0.040513f, -0.083960f},
    { 0.120874f,  0.055753f, -0.025988f, -0.059552f},
    { 0.079912f,  0.007894f, -0.085380f, -0.114587f},
    { 0.036856f, -0.039331f, -0.104237f, -0.069116f},
    { 0.008526f, -0.064273f, -0.048312f, -0.038595f},
    { 0.033461f, -0.028956f, -0.066505f,  0.038722f},
    {-0.042064f, -0.043989f, -0.100653f, -0.071550f},
    {-0.015342f, -0.064850f, -0.065675f, -0.122769f},
    {-0.006581f, -0.004919f, -0.113564f, -0.145753f},
    { 0.008273f, -0.070702f, -0.164998f, -0.095541f},
    {-0.001698f, -0.063744f, -0.129971f, -0.011162f},
    {-0.048471f, -0.087500f, -0.111006f, -0.161823f},
    {-0.032193f, -0.091955f, -0.080642f,  0.012288f},
    {-0.095873f, -0.015986f, -0.072722f, -0.101745f},
    {-0.079477f, -0.082060f, -0.203008f, -0.100297f},
    {-0.023883f, -0.064022f, -0.168341f, -0.211739f},
    {-0.070530f, -0.103547f, -0.123858f,  0.055049f},
    {-0.033503f, -0.076812f, -0.016287f,  0.044159f},
    {-0.088427f, -0.161682f, -0.058579f,  0.013873f},
    {-0.083068f, -0.168222f, -0.016773f, -0.080209f},
    {-0.080548f, -0.139090f,  0.030544f,  0.007171f},
    {-0.117482f, -0.083718f,  0.027074f, -0.003674f},
    {-0.163085f, -0.156856f, -0.012618f, -0.022329f},
    {-0.176540f, -0.113042f, -0.020148f,  0.051770f},
    {-0.153891f, -0.199293f, -0.043244f,  0.028331f},
    {-0.107822f, -0.150615f,  0.016430f,  0.092919f},
    {-0.137676f, -0.183224f,  0.066026f,  0.029343f},
    {-0.191106f, -0.099250f,  0.045370f,  0.004084f},
    {-0.237042f, -0.130815f, -0.022543f, -0.029428f},
    {-0.201014f, -0.053591f, -0.007305f, -0.033547f},
    {-0.249286f, -0.228408f,  0.005002f,  0.007146f},
    {-0.206509f, -0.211998f, -0.061352f, -0.047233f},
    {-0.255702f, -0.135114f,  0.076375f,  0.036630f},
    {-0.296271f, -0.073946f, -0.007273f, -0.019601f},
    {-0.302917f, -0.175111f, -0.070024f, -0.043905f},
    {-0.239275f, -0.043962f, -0.084982f, -0.067446f},
    {-0.254583f, -0.294720f, -0.088762f, -0.070451f},
    {-0.205583f, -0.238996f, -0.124753f,  0.033076f},
    {-0.205583f, -0.215882f, -0.028472f,  0.118679f},
    {-0.153640f, -0.204464f, -0.039654f, -0.134441f},
    {-0.145929f, -0.191970f, -0.175308f,  0.021366f},
    {-0.149348f, -0.212569f, -0.118324f,  0.103812f},
    {-0.166397f, -0.220581f, -0.265260f, -0.029113f},
    {-0.164171f, -0.231262f, -0.258828f,  0.061427f},
    {-0.200198f, -0.263453f, -0.212016f,  0.115359f},
    {-0.130088f, -0.212168f, -0.202368f,  0.118563f},
    {-0.206387f, -0.078075f, -0.227856f, -0.111165f},
    {-0.129605f, -0.176848f, -0.241584f, -0.259900f},
    {-0.176826f, -0.045901f, -0.141712f, -0.209345f},
    {-0.351173f, -0.031097f, -0.133935f, -0.182412f},
    {-0.164232f,  0.027006f, -0.014039f, -0.053567f},
    {-0.171037f, -0.025924f,  0.030972f,  0.017329f},
    {-0.080862f, -0.021577f,  0.007652f,  0.063968f},
    {-0.061788f,  0.042024f, -0.018783f, -0.057979f},
    {-0.110311f,  0.054760f,  0.031446f, -0.006710f},
    {-0.136637f,  0.022171f,  0.084991f,  0.028039f},
    {-0.254471f, -0.004376f,  0.078034f,  0.033649f},
    {-0.234464f,  0.088157f,  0.040999f,  0.002639f},
    {-0.037095f,  0.059443f,  0.072180f,  0.015027f},
    {-0.046841f, -0.004813f,  0.088266f,  0.038786f},
    {-0.086782f,  0.120100f,  0.082655f,  0.020271f},
    {-0.118361f, -0.069242f,  0.094867f,  0.039200f},
    {-0.023342f, -0.084303f,  0.052684f,  0.017093f},
    {-0.014194f,  0.001012f,  0.011946f,  0.074125f},
    {-0.015342f,  0.076396f,  0.022365f, -0.028001f},
    { 0.027706f,  0.037047f,  0.107573f,  0.060815f},
    { 0.030615f,  0.040664f,  0.010467f,  0.074289f},
    { 0.038646f,  0.115584f,  0.069627f,  0.007642f},
    { 0.096463f,  0.069818f,  0.062494f,  0.015413f},
    { 0.054834f,  0.065232f,  0.054286f,  0.110088f},
    { 0.152312f,  0.092371f,  0.026420f, -0.013184f},
    { 0.144264f,  0.123438f,  0.080131f,  0.023233f},
    { 0.124405f,  0.009943f, -0.148477f, -0.205184f}
};

static const float *lsf_codebooks_16k[] = {
    lsf_cb1_16k[0], lsf_cb2_16k[0], lsf_cb3_16k[0], lsf_cb4_16k[0],
    lsf_cb5_16k[0]
};

#endif /* AVCODEC_SIPR16KDATA_H */
