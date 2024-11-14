#pragma once
#define to_vec4(r, g, b, a) ImColor(r / 255.f, g / 255.f, b / 255.f, a)



namespace settings {

	namespace thread {

		inline bool psygun = false;
		inline bool invisible = false;
		inline bool repairveh = false;
		inline bool startsilent = false;
		inline bool lock_menu_layout = false;
		inline bool windows_flags = false;
	}


	inline float size = 1.5f;
	inline float particleColour[4] = { 255.f, 255.f, 255.f, 255.f };

	inline ImColor particle = ImColor(255, 255, 255, 255);

	namespace colors {

		inline ImVec4 accent_color = ImColor(255, 255, 255, 255); // GENERAL COLOR

		namespace background {

			inline ImVec4 text_background = ImColor(255, 0, 0, 255);
			inline ImVec4 background = ImColor(255, 0, 0, 255);
			inline ImVec4 border = ImColor(255, 0, 0, 255);
		}

		namespace child {

			inline ImVec4 background_cap = ImColor(255, 0, 0, 255);
			inline ImVec4 background = ImColor(255, 0, 0, 255);
			inline ImVec4 border = ImColor(255, 0, 0, 255);
			inline ImVec4 name = ImColor(255, 255, 255, 255);
		}

		namespace checkbox {

			inline ImVec4 background = ImColor(0, 255, 0, 255);
			inline ImVec4 circle_inactive = ImColor(40, 40, 40, 255);
		}

		namespace slider {

			inline ImVec4 accent = ImColor(145 / 2, 92 / 2, 255 / 2, 255); // GENERAL COLOR

			inline ImVec4 background = ImColor(25, 25, 25, 255);
			inline ImVec4 circle_inactive = ImColor(40, 40, 40, 255);
		}

		namespace picker {

			inline ImVec4 text_active = ImColor(255, 255, 255, 255);

			inline ImVec4 background = ImColor(13, 13, 13, 255);
			inline ImVec4 border = ImColor(22, 22, 22, 255);
		}

		namespace combo {

			inline ImVec4 background = ImColor(25, 25, 25, 255);
			inline ImVec4 text_active = ImColor(255, 255, 255, 255);

			namespace selectable {

				inline ImVec4 text_hov = ImColor(150, 150, 150, 255);
				inline ImVec4 text = ImColor(100, 100, 100, 255);

			}

		}

		namespace button {

			inline ImVec4 background_active = ImColor(255, 35, 35, 255);
			inline ImVec4 background_hov = ImColor(30, 30, 30, 255);
			inline ImVec4 background = ImColor(255, 0, 0, 255);
		}

		namespace input {

			inline ImVec4 text_selected_bg = ImColor(45, 45, 45, 100);
			inline ImVec4 background = ImColor(25, 25, 25, 255);

			inline ImVec4 text_active = ImColor(255, 255, 255, 255);
			inline ImVec4 text_hov = ImColor(150, 150, 150, 255);
			inline ImVec4 text = ImColor(100, 100, 100, 255);

		}

		namespace tabs {

			inline ImVec4 border = ImColor(22, 22, 22, 255);

			inline ImVec4 background_active = ImColor(25, 25, 25, 255);
			inline ImVec4 background_inactive = ImColor(15, 15, 15, 255);

			inline ImVec4 text_active = ImColor(255, 255, 255, 255);
			inline ImVec4 text_hov = ImColor(150, 150, 150, 255);
			inline ImVec4 text = ImColor(100, 100, 100, 255);
		}

		namespace scrollbar {
			inline ImVec4 background = ImColor(0, 21, 255, 255);
		}

		namespace keybind {
			inline ImVec4 background = ImColor(0, 21, 255, 255);

			inline ImVec4 text_active = ImColor(0, 21, 255, 255);
			inline ImVec4 text_hov = ImColor(0, 21, 255, 255);
			inline ImVec4 text = ImColor(0, 21, 255, 255);
		}




	}

	namespace style {
		inline bool particles = true;

		namespace background {

			inline ImVec2 size = ImVec2(800, 700);
			inline float rounding = 9.f * settings::size;
			inline float border_size = 1.0f * settings::size;
		}

		namespace child {
			inline float rounding = 9.f * settings::size;
			inline float border_size = 1.0f * settings::size;
		}

		namespace checkbox {

			inline float rounding = 25.f * settings::size;
			inline bool shadows = true;
		}

		namespace slider {

			inline float rounding = 25.f * settings::size;
			inline bool shadows = true;
		}

		namespace picker {

			inline float border_size = 1.0f * settings::size;
			inline float rounding = 4.f * settings::size;
			inline bool shadows = true;
		}

		namespace combo {

			inline float rounding = 4.f * settings::size;
		}

		namespace button {

			inline float rounding = 4.f * settings::size;
		}

		namespace input {

			inline ImVec2 text_padding = ImVec2(10, 6.0f);
			inline float rounding = 4.f * settings::size;
			inline float text_selected_rounding = 4.f * settings::size;
		}

		namespace tabs {

			inline float rounding = 4.f * settings::size;
			inline float border_size = 1.0f * settings::size;

		}

		namespace keybind {

			inline float rounding = 4.f * settings::size;

		}

	}

	namespace fonts {
		inline ImFont* icon = nullptr;
		inline ImFont* icon_big = nullptr;
		inline ImFont* tab_text = nullptr;
		inline ImFont* cheat_name = nullptr;
	}
	namespace Teleport
	{
		inline char searchString[256] = "";
		inline int Index = -1;
	}
	namespace SilentAim
	{
		inline bool Enabled = false;
		inline bool MagicBullets = false;
		inline bool IgnoreDead = false;
		inline bool IgnoreNpcs = false;
		inline bool IgnoreInVisible = true; // AUF TRUE LASSEN
		inline bool IgnoreFriends = false;
		inline bool DrawFoV = false;
		inline int TargetPoint = 0;
		inline int Bullet = 0;
		inline float BulletSpeed = 300.f;
		inline int MaxDistance = 150;
		inline int MaxFoV = 10;
		inline int MagicMaxFoV = 5;
		inline int FoVStyle = 0;
		inline int AimKey = 2;
		inline int MagicKey = 0;
		inline int MissChange = 0;
		inline int selectedAmmo = 0;
		inline const char* TargetPointItems[]{ "Head", "Neck", "Pelvis" };
		inline const char* BulletItems[]{ "Current Weapon", "Explosive Bullet" };
		inline ImColor SilentFoVColor = ImColor(255, 255, 255, 255);
		inline float SilentFoVColorTemp[4] = { 255.f, 255.f, 255.f, 255.f };
	}
	namespace aimbot {

		inline bool enabled = false;
		inline bool silentaim = false;
		inline bool silentaimenable = false;
		inline int hitchance = 0;

		inline bool Magicccc = false;

		inline bool TriggerBot = false;
		inline bool ignorepeds = true;

		inline int hotkey = 2;

		inline int hotkeysilent = 0;

		inline int aimbot_fov = 20;
		inline int aimbot_fov1 = 5;
		inline int smoothhorizontal = 2;
		inline int smoothvertical = 2;

		inline int aimbot_target = 0;
		inline bool ignore_peds = false;
		inline const char* aimbot_target_list[]{ "All", "Only Player", "Only Ped" };

		inline int aimbot_distance = 150;

		inline int aimbot_smooth = 0;
		inline bool ignor = true;
		inline bool distancetestiz = true;


		inline int aimbot_bone = 0;
		inline const char* aimbot_bones_list[]{ "Head", "Neck", "Body" };

		inline int aimbot_type = 1;
		inline const char* aimbot_type_list[]{ "Mouse", "Memory","Silent Aim" };

		inline bool target_lock = false;

		inline bool overide_target = false;
		inline bool view_aim_point = false;

		inline bool auto_shoot = false;

		inline int aimbot_selection = 0;

	}


	namespace radar {

		bool enabled = false;
		bool enable2DRadar = false;

	}

	namespace visuals {

		extern int colorsheme = 0;
		inline const char* colorsheme_list[]{ "White", "Blue", "Pink" };


		//constexpr int cfg_name_max_length = 256;
		//char cfg_name[cfg_name_max_length] = "";
		//std::vector<std::string> saved_configs;
		//int selected_config_index = -1; // IST DOCH EGAL WO DU HS
		//inline char rename[255] = "";


		inline int menu_alpha = 255;
		inline bool enabled = false;

		inline int master_switch = 0;

		inline bool filled = false;
		inline int healthbar_style = 1; //bro dein source geordnet ; skull 
		inline bool box = false;
		inline int box_round = 0;
		inline bool fiveguardbypass = false;

		inline bool skeleton = false;
		inline bool skeleton1 = false;
		inline bool streamproof = false;
		inline bool armorbar = false;
		inline bool corner = false;
		inline bool weapon_name = false;
		inline bool hitsound = false;
		char hitsound_path[50] = "C:\\user\\cod.ogg";
		inline bool object = false;
		inline bool playername = false;
		inline bool filled_color_visible = false;
		inline bool box_color_visible = false;
		inline bool invisible_check_type = false;
		inline bool Draw_LocalPlayer = false;
		inline bool friend_visuals = false;
		inline bool player_name_color_visible = false;
		inline bool player_name_color = false;
		inline bool invisible_color = false;
		inline bool box_color = false;
		inline bool vehicle = false;
		//inline bool object = false;
		inline bool visible_check_type = false;
		inline bool friend_visuals_check_type = false;
		inline bool playerid = false;
		inline bool friends_colors = false;
		inline bool healthbar = false;
		inline bool visible_check = false;
		inline bool snapline = false;
		inline bool esp_type = false;
		inline bool circle_at_middle = false;
		inline bool dont_draw_health = false;
		inline bool dont_draw_distance = false;

		inline bool draw_dead = false;
		inline bool only_alive = false;
		inline bool aim_dead = false;
		inline int dead_opacity = 50;
		inline bool draw_ped = false;
		inline bool draw_self = false;


		inline bool watermark = true;
		inline int render_distance = 200;
		inline int vehicle_distance = 200;
		inline int lootbag_distance = 200;
		inline int skeleton_numbers = 0;
		inline int skeleton_numbers1 = 0;
		inline int skeleton_numbers2 = 0;
		inline int skeleton_numbers3 = 0;
		inline int skeleton_numbers4 = 0;
		inline int skeleton_numbers5 = 0;
		inline int skeleton_numbers6 = 0;
		inline int skeleton_numbers7 = 0;
		inline int skeleton_numbers8 = 0;
		inline int skeleton_numbers9 = 0;
		inline bool preview_target = false;
		inline int preview_style = 0;
		inline bool draw_fov = false;
		inline bool filled_fov = false;

		inline const char* ssxs[]{ "Dot", "Line", "Both" };

		inline bool crosshair = false;

		inline bool language = false;
		inline bool darkmode = true;
		inline bool particles = true;
		inline int crossahir_style = 0;
		inline int HealthbarStyle = 0;


		inline const char* sss[]{ "FiveM Reshade Crosshair", "Dot", "Cross" };

		inline int OutlinedText = 0;


		inline const char* OutlinedTextType[]{ "Outlined Text 1", "Outlined Text 2" };




		inline float BoxSize = 1;
		inline int FillBoxAlpha = 70;

		inline ImColor boxcolor = ImColor(255, 255, 255, 255);
		inline ImColor accentcolor_particle = ImColor(255, 255, 255, 255);
		inline ImColor boxcolordead = ImColor(255, 0, 0, 255);
		inline ImColor filledcolor = ImColor(255, 255, 255, 255);
		inline ImColor cornercolor = ImColor(33, 140, 176, 255);
		inline ImColor namecolor = ImColor(255, 255, 255, 255);
		inline ImColor skeleton_color = ImColor(255, 255, 255, 255);
		inline ImColor skeletonfriend_color = ImColor(255, 255, 255, 255);
		inline ImColor weapon_color = ImColor(255, 255, 255, 255);
		inline ImColor snapeline_color = ImColor(255, 255, 255, 255);
		inline ImColor distance_color = ImColor(255, 255, 255, 255);
		inline ImColor head_color = ImColor(255, 255, 255, 255);
		inline ImColor crosshair_color = ImColor(255, 255, 255, 255);
		inline ImColor preview_target_colors = ImColor(255, 255, 255, 255);
		inline ImColor fov_color = ImColor(255, 255, 255, 255);
		//___//
		inline ImColor vehicle_esp_color = ImColor(255, 255, 255, 255);
		inline ImColor vehiclebroken_esp_color = ImColor(255, 255, 255, 255);

		inline ImColor vehicle_esp_health = ImColor(255, 255, 255, 255);
		inline ImColor vehicle_esp_marker = ImColor(255, 255, 255, 255);
		inline ImColor vehicle_esp_distance = ImColor(255, 255, 255, 255);
		//___//
		inline ImColor bag_esp_color = ImColor(255, 255, 255, 255);
		inline ImColor bagdistance_esp_color = ImColor(255, 255, 255, 255);
		inline ImColor bagmarker_esp_color = ImColor(255, 255, 255, 255);
		float accent_color[4] = { 0, 21, 255, 255 };
		ImColor get_accent_color(float a = 1.f) {

			return to_vec4(accent_color[0], accent_color[1], accent_color[2], a);
		}


	}

	namespace vehicle {

		inline bool enabled = false;
		inline bool hornboost = false;
		inline bool vehicleprimcolor = false;
		inline bool driftmode = false;
		inline bool vehclip = false;
		inline bool key_warp = false;
		inline bool repair = false;
		inline bool vehicle_speed_bool = false;
		inline bool vehicle_repair = false;
		inline int vehicle_speed_value = 0;
		inline bool vehicle_esp = false;
		inline bool warp_on_key = false;
		inline bool vehicle_name = false;
		inline bool goofyfly = false;
		inline bool vehicle_health = false;
		inline bool vehicle_distance = false;
		inline bool vehicle_snapline = false;
		inline bool vehicle_marker = false;


	}

	namespace object {

		inline bool object_esp = false;

		inline bool object_name = false;
		inline bool object_distance = false;
		inline bool object_snapline = false;
		inline bool object_marker = false;

	}

	namespace attachments {

		inline bool enabled = false;
		inline bool explosiveammo = false;
		inline bool supressor = false;

	}

	namespace weapon {

		inline bool enabled = false;
		inline bool Pattern = false;
		inline int psygunkey = 0;
		static int weapon_current = 0;
		inline const char* weapon_list[]{ "Dagger", "Bat", "Knife", "Machete", "Pistol", "Pistol MK2", "Combat Pistol", "AP Pistol", "Stungun", "Pistol 50", "SNS PISTOL", "SNS Pistol MK2", "Heavy Pistol", "Vintage Pisol", "Flare Gun", "Marksman Pistol", "Revolver", "Revolver MK2", "Double Action", "Micro Smg", "Smg", "Smg MK2", "Assault Smg", "Combat PDW", "Machine Pistol", "Mini Smg", "Pump Shotgun", "Pump Shotgun MK2", "Sawnoff Shotgun", "Assault Shotgun", "Bullpup Shotgun", "Musket", "Heavy Shotgun", "DB Shotgun", "Auto Shotgun", "Assault Rifle", "Assault Rifle MK2", "Carbine Rifle", "Carbine Rifle MK2", "Advanced Rifle", "Special Carbine", "Special Carbine MK2", "Bullpup Rifle", "Bullpup Rifle MK2", "Compact Rifle", "Machine Gun", "Combat MG", "Combat MG MK2", "GUSENBERG", "Sniper Rifle", "AWP", "AWP MK2", "Marksman Rifle", "Marksman Rifle MK2", "RPG", "Grenade Launcher", "MiniGun", "FireWork", "RailGun", "Homing Launcher", "Compact Launcher", "Grenade", "BZGAS", "Smoke Grenade", "Flare", "Molotov", "Sticky BOMB", "Prox Mine", "SnowBall", "Pipe Bomb", "Ball", "Petrol Can", "Fire Extinguisher", "Parachute" };
		inline bool no_recoil = false;
		inline bool no_reload = false;
		inline bool no_spread = false;
		inline bool explosiveammo = false;
		inline bool explo_meele = false;
		inline bool weaponfireammo = false;
		inline bool infinite_range = false;

		inline bool infite_ammo = false;
		inline bool set_ammo = false;
		inline bool damage_boost = false;

		inline float recoil_value = 0;
		inline int reload_value = 100;
		inline int spread_value = 0;
		inline int weapon_range = 1000;
		inline int ammo_to_set = 0;
		inline bool int_damage = 999;

		inline bool custom_fov = false;
		inline int scope_fov_value = 50;

		inline bool r_custom_fov = false;
		inline int r_scope_fov_value = 90;

		inline bool s_custom_fov = false;
		inline int s_scope_fov_value = 90;
	}


	namespace self {

		inline bool enabled = false;
		inline bool autoarm = false;
		inline bool invisible = false;
		inline bool freecam = false;
		inline bool testcar = false;
		inline bool tptowaypoint = false;
		inline bool randomoutfit = false;
		inline bool godmode = false;
		inline bool localplayer = false;
		inline bool PedList = false;
		inline bool fovcganger = false;
		inline bool NoRagDoll = false;
		inline bool Invisible = false;
		inline bool Infinite_Armor = false;
		inline bool nocol = false;
		inline bool isNoclipWorking = true;
		inline bool pysgun = true;
		inline bool blockinput = false;
		inline int  FovValue = 100;


		inline bool fastrun = false;
		inline bool revive = false;
		inline bool semigodmode = false;
		inline bool fiveguardbypass = false;
		inline bool set_armor = false;
		inline bool set_health = false;
		inline bool set_revive = false;
		inline int give_health_amnt = 100;
		inline int give_armor_amnt = 100;
		inline bool give_armor = false;
		inline bool noclip = false;
		inline int health = 100;
		inline int armor = 100;
		inline int teleport_to_waypoint = false;
		inline int never_wanted = false;
		inline bool give_health = false;
		inline int infinite_stamina = false;
		inline bool no_ragdoll = false;
		inline int desync = false;
		inline bool no_hs = false;



		inline float RunSpeedToSet = 1;
		inline int hotkey_noclip = 0;
		inline int hotkey_vehclip = 0;
		inline int hotkey_blockmenu = 0;
		inline int hotkey_autofarm = 0;
		inline int HotkeyArmor = 0;
		inline int HotkeyHealth = 0;

		inline int hotkey_forward = 0x57;
		inline int hotkey_left = 0x41;
		inline int hotkey_right = 0x44;
		inline int hotkey_backward = 0x53;
		inline bool noclipspeed = false;
		inline float noclipspeedlol = 1.00000f;



		inline int hotkey_lootbagesp = 0;
		inline int hotkey_vehicleesp = 0;
		inline int hotkey_explosiveammo = 0;
		inline int hotkey_damage = 0;
		inline int hotkey_recoil = 0;
		inline int hotkey_vehicle = 0;
		inline int hotkey_revive = 0;

	}


	namespace tempcolor
	{
		inline float box[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float healthbar[4] = { 17.f, 255.f, 0.f, 255.f };
		inline float armorbar[4] = { 0.f, 21.f, 255.f, 255.f };
		inline float particle[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float filled[4] = { 255.f, 255.f, 255.f, 125.f };
		inline float corner[4] = { 33.f, 140.f, 176.f, 255.f };
		inline float skeleton[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float weapon[4] = { 33.f, 140.f, 176.f, 255.f };
		inline float head[4] = { 33.f, 140.f, 176.f, 255.f };
		inline float name[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float snapline[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float distance[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float crosshair[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float preview_target[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float fov_color[4] = { 255.f, 255.f, 255.f, 255.f };
		//_//_//_/_/_/_/_/_/_/
		inline float bag_espg[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float vehicle_esp[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float bagmarker[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float bagdistanccolor[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float vehiclebrokencolor[4] = { 255.f, 0.f, 0.f, 255.f };
		inline float vehicleesphealth[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float vehiclemarker[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float vehicledistance[4] = { 255.f, 255.f, 255.f, 255.f };

	}



}

