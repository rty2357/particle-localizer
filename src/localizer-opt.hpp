/*
 * localizer_opt.hpp
 *
 *  Created on: 2011/07/28
 *      Author: tyamada
 */

#ifndef _LOCALIZER_OPT_HPP_
#define _LOCALIZER_OPT_HPP_


#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include <ssmtype/spur-odometry.h>
#include <ssmtype/pws-motor.h>

#include "localizer-conf.hpp"

namespace Localizer {

	class particle_localizer_option
	{
		// ---> declaration
	public:
		typedef struct {
			bool random;
			bool start_at;
			bool debug;
		} operation_mode;
		operation_mode op_mode;
		// <--- declaration
	public:
		configure_parameters *param;



		// ---> constructor
	public:
		particle_localizer_option(){ init(); }
		particle_localizer_option(configure_parameters *p) : param(p) { init(); }
		~particle_localizer_option(){}
	private:
		void init();
	public:
		int set(configure_parameters *p){
			param = p;
			return 0;
		}
		// <--- constructor

		// ---> operation
	public:
		bool get_option(int aArgc, char **aArgv);
		// <--- operation
	};


// alias
	typedef	particle_localizer_option
			particle_opt;

	const char particle_filter[] = "localizer";

	// default parameter
	const particle_localizer_option::operation_mode __PARTICLE_OPTION_DEFPARAM__ = {
			false,	// random sampling
			false,	// start-at
			false,	// debug
	};


	const char ShortOpt[] = "hk:g:ar:w::d";

	const struct option LongOpt[] = {
		{"help", 				no_argument,		0, 'h'},
		{"config",				required_argument,	0,	'g'},
		{ConfIni_KFile.token,	required_argument,	0,	'k'},
		{"start-at",			no_argument,		0,	'a'},
		{"wide-sampling",		optional_argument,	0,	'w'},
		{"debug",				no_argument,		0,	'd'},
		{0, 0, 0, 0}	// end of array
	};



	inline void Localizer::particle_opt::init()
	{
		::memcpy(&op_mode, &Localizer::__PARTICLE_OPTION_DEFPARAM__, sizeof(operation_mode));
	}

	inline bool Localizer::particle_opt::get_option(int aArgc, char **aArgv)
	{
		int opt;

		while(1){
			optarg = 0;
			opt = ::getopt_long(aArgc, aArgv, Localizer::ShortOpt, Localizer::LongOpt, 0);
			if(opt < 0)	break;

			switch(opt){
			case 'k': ::strcpy(param->kfile.value, optarg); break;
			case 'g':
			{
				gnd_conf_fs conf_fs;
				if( conf_fs.read(optarg) < 0 ){
					::fprintf(stderr, " ... [\x1b[1m\x1b[31mERROR\x1b[30m\x1b[0m]: -g option, Fail to read configure file\n");
					return -1;
				}
				if( get_config_param(&conf_fs, param) < 0){
					::fprintf(stderr, " ... [\x1b[1m\x1b[31mERROR\x1b[30m\x1b[0m]: -g option, configure file syntax error\n");
					return -1;
				}
			} break;

			case 'w': {
				if( !optarg || !::strcmp(optarg, "on") )			op_mode.random = true;
				else if( !::strcmp(optarg, "off") )					op_mode.random = false;
			} break;
			case 'a': op_mode.start_at = true; break;
			case 'd': op_mode.debug = true; break;

			case 'h':
			{
				int i = 0;
				fprintf(stderr, "\t\x1b[1mNAME\x1b[0m\n");
				fprintf(stderr, "\t\t\x1b[1mlocarizer\x1b[0m - localizer with particle filter\n");
				fprintf(stderr, "\n");

				fprintf(stderr, "\t\x1b[1mSYNAPSIS\x1b[0m\n");
				fprintf(stderr, "\t\t\x1b[1mlocarizer\x1b[0m [\x1b[4mOPTIONS\x1b[0m]\n");
				fprintf(stderr, "\n");

				fprintf(stderr, "\t\x1b[1mDISCRIPTION\x1b[0m\n");
				fprintf(stderr, "\t\t\x1b[1mlocarizer\x1b[0m estimates robot position with particle filter.\n");
				fprintf(stderr, "\t\t\x1b[1mlocarizer\x1b[0m write estimate-position(\"\x1b[4m%s\x1b[0m\") and particles(\"\x1b[4m%s\x1b[0m\") into \x1b[4mssm\x1b[0m.\n", SNAME_ADJUST, SNAME_PARTICLES);
				fprintf(stderr, "\t\t\x1b[1mlocarizer\x1b[0m requires ssm-data \"\x1b[4m%s\x1b[0m\" for tracking robot position, \n", SNAME_PWS_MOTOR);
				fprintf(stderr, "\t\tand ssm-data \"\x1b[4m%s\x1b[0m\" for estimate position.\n", SNAME_PARTICLES_EVALUATION);

				fprintf(stderr, "\n");
				fprintf(stderr, "\t\x1b[1mOPTIONS\x1b[0m\n");
				fprintf(stderr, "\t\t\x1b[1m-%c\x1b[0m, \x1b[1m--%s\x1b[0m\n", Localizer::LongOpt[i].val,  Localizer::LongOpt[i].name);
				fprintf(stderr, "\t\t\tprint help\n");
				fprintf(stderr, "\n");
				i++;

				fprintf(stderr, "\t\t\x1b[1m-%c\x1b[0m, \x1b[1m--%s\x1b[0m\n", Localizer::LongOpt[i].val,  Localizer::LongOpt[i].name);
				fprintf(stderr, "\t\t\tread configure file\n");
				fprintf(stderr, "\n");
				i++;

				fprintf(stderr, "\t\t\x1b[1m-%c\x1b[0m, \x1b[1m--%s\x1b[0m\n", Localizer::LongOpt[i].val,  Localizer::LongOpt[i].name);
				fprintf(stderr, "\t\t\tread kinematics parameter file\n");
				fprintf(stderr, "\n");
				i++;

				fprintf(stderr, "\t\t\x1b[1m-%c\x1b[0m, \x1b[1m--%s\x1b[0m\n", Localizer::LongOpt[i].val,  Localizer::LongOpt[i].name);
				fprintf(stderr, "\t\t\tturn on random sampling mode\n");
				fprintf(stderr, "\n");
				i++;

				fprintf(stderr, "\t\t\x1b[1m-%c\x1b[0m, \x1b[1m--%s\x1b[0m\n", Localizer::LongOpt[i].val,  Localizer::LongOpt[i].name);
				fprintf(stderr, "\t\t\tset id of reading ssm-data named \"\x1b[4mpws-motor\x1b[0m\"\n");
				fprintf(stderr, "\n");
				i++;

				fprintf(stderr, "\t\t\x1b[1m-%c\x1b[0m, \x1b[1m--%s\x1b[0m\n", Localizer::LongOpt[i].val,  Localizer::LongOpt[i].name);
				fprintf(stderr, "\t\t\tturn on debug-log file out\n");
				fprintf(stderr, "\n");
				i++;

				fprintf(stderr, "\t\t\x1b[1me.g.\x1b[0m with log) %s -k knm.conf\n", aArgv[0]);
				return false;
			}break;
			}
		}
		return true;
	}

};


#endif /* LOCALIZER_OPT_HPP_ */
