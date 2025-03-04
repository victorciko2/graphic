/*********************************************************************************
Copyright (C) 2014 Adrian Jarabo (ajarabo@unizar.es)
Copyright (C) 2014 Diego Gutierrez (diegog@unizar.es)
All rights reserved.

This is an educational Ray Tracer developed for the course 'Informatica Grafica'
(Computer Graphics) tought at Universidad de Zaragoza (Spain). As such, it does not
intend to be fast or general, but just to provide an educational tool for undergraduate
students.

This software is provided as is, and any express or implied warranties are disclaimed.
In no event shall copyright holders be liable for any damage.
**********************************************************************************/
#include <random>
#include <cmath>
#include "PhotonMapping.h"
#include "World.h"
#include "Intersection.h"
#include "Ray.h"
#include "BSDF.h"
#include <iostream>

int MAX_NB_BOUNCES = 20;
//*********************************************************************
// Compute the photons by tracing the Ray 'r' from the light source
// through the scene, and by storing the intersections with matter
// in the lists 'xx_photons', storing the diffuse (global) and caustic
// photons respectively. For efficiency, both are computed at the same
// time, since computing them separately would result into a lost of
// several samples marked as caustic or diffuse.
// Same goes with the boolean 'direct', that specifies if direct 
// photons (from light to surface) are being stored or not. 
// The initial traced photon has energy defined by the tristimulus
// 'p', that accounts for the emitted power of the light source.
// The function will return true when there are more photons (caustic
// or diffuse) to be shot, and false otherwise.
//---------------------------------------------------------------------
bool PhotonMapping::trace_ray(const Ray& r, const Vector3 &p,
	std::list<Photon> &global_photons, std::list<Photon> &caustic_photons, bool direct, bool direct_only)
{
#ifndef MAX_PHOTON_ITERATIONS
#define MAX_PHOTON_ITERATIONS 20
#endif



	//Check if max number of shots done...
	if (++m_nb_current_shots > m_max_nb_shots)
	{
		return false;
	}

	// Compute irradiance photon's energy
	Vector3 energy(p);

	Ray photon_ray(r);
	photon_ray.shift();

	bool is_caustic_particle = false;

	//Iterate the path
	while (1)
	{
		// Throw ray and update current_it
		Intersection it;
		world->first_intersection(photon_ray, it);

		if (!it.did_hit())
			break;

		//Check if has hit a delta material...
		if (it.intersected()->material()->is_delta())
		{
			// If delta material, then is caustic...
			// Don't store the photon!
			is_caustic_particle = true;
		}
		else if (photon_ray.get_level() > 0 || direct || direct_only)
		{
			//If non-delta material, store the photon!
			if (is_caustic_particle)
			{
				//If caustic particle, store in caustics
				if (caustic_photons.size() < m_nb_caustic_photons)
					caustic_photons.push_back(Photon(it.get_position(), photon_ray.get_direction(), energy));
			}
			else
			{
				//If non-caustic particle, store in global
				if (global_photons.size() < m_nb_global_photons)
					global_photons.push_back(Photon(it.get_position(), photon_ray.get_direction(), energy));
			}
			is_caustic_particle = false;
		}

		Real pdf;

		Vector3 surf_albedo = it.intersected()->material()->get_albedo(it);
		Real avg_surf_albedo = surf_albedo.avg();

		Real epsilon2 = static_cast<Real>(rand()) / static_cast<Real>(RAND_MAX);
		while (epsilon2 < 0.)
			epsilon2 = static_cast<Real>(rand()) / static_cast<Real>(RAND_MAX);

		if (epsilon2 > avg_surf_albedo || photon_ray.get_level() > MAX_PHOTON_ITERATIONS)
			break;

		if (direct_only && !is_caustic_particle && photon_ray.get_level() == 0)
			break;

		// Random walk's next step
		// Get sampled direction plus pdf, and update attenuation
		it.intersected()->material()->get_outgoing_sample_ray(it, photon_ray, pdf);

		// Shade...
		energy = energy * surf_albedo;
		if (!it.intersected()->material()->is_delta())
			energy *= dot_abs(it.get_normal(), photon_ray.get_direction()) / 3.14159;

		energy = energy / (pdf*avg_surf_albedo);
	}

	if (caustic_photons.size() == m_nb_caustic_photons &&
		global_photons.size() == m_nb_global_photons)
	{
		m_max_nb_shots = m_nb_current_shots - 1;
		return false;
	}

	return true;
}

//*********************************************************************
// TODO: Implement the preprocess step of photon mapping,
// where the photons are traced through the scene. To do it,
// you need to follow these steps for each shoot:
//  1 - Sample a world's light source in the scene to create
//		the initial direct photon from the light source.
//	2 - Trace the photon through the scene storing the inter-
//		sections between the photons and matter. You can use
//		the function 'trace_ray' for this purpose.
//	3 - Finally, once all the photons have been shot, you'll
//		need to build the photon maps, that will be used later
//		for rendering. 
//		NOTE: Careful with function
//---------------------------------------------------------------------
void PhotonMapping::preprocess()
{
	random_device rd;
	mt19937 gen = mt19937(rd());
	uniform_real_distribution<float> distribution = uniform_real_distribution<float>(0.0, 1.0);
	float phi, theta;
	float total_intensity = 0;
	int num_samples;
	LightSource* light = nullptr;
	list<Photon> global_photons, caustic_photons;
	Vector3 direction = Vector3(0, 0, 0);
	for (int i = 0; i < world->light_source_list.size(); i++) {
		total_intensity += world->light_source_list[i]->get_intensities().length();
	}
	for (int i = 0; i < world->light_source_list.size(); i++) {
		light = world->light_source_list[i];
		num_samples = m_nb_global_photons * (light->get_intensities().length() / total_intensity);

		for(int i = 0; i < num_samples; i++){
			theta = 2 * M_PI * distribution(gen);
			phi = acos(1 - 2 * distribution(gen));
			direction = Vector3(sin(phi) * cos(theta), sin(phi) * sin(theta), cos(phi));
			direction.normalize();
			trace_ray(Ray(light->get_position(), direction), 4 * M_PI * light->get_intensities() / num_samples,
					global_photons, caustic_photons, false, false);
		}
	}	

	bool balanced = true;
	std::list<Photon>::const_iterator iterator;
	Photon photon;
	for (iterator = global_photons.begin(); iterator != global_photons.end(); ++iterator) {
		photon = *iterator;
		std::vector<Real> photon_position(photon.position.data, photon.position.data + 3);
		m_global_map.store(photon_position, photon);
		balanced = false;
	}
	if (!balanced) {
		m_global_map.balance();
		balanced = true;
	}
	for (iterator = caustic_photons.begin(); iterator != caustic_photons.end(); ++iterator) {
		photon = *iterator;
		std::vector<Real> photon_position(photon.position.data, photon.position.data + 3);
		m_caustics_map.store(photon_position, photon);
		balanced = false;
	}
	if (!balanced) {
		m_caustics_map.balance();
	}
}


//*********************************************************************
// TODO: Implement the function that computes the rendering equation 
// using radiance estimation with photon mapping, using the photon
// maps computed as a proprocess. Note that you will need to handle
// both direct and global illumination, together with recursive the 
// recursive evaluation of delta materials. For an optimal implemen-
// tation you should be able to do it iteratively.
// In principle, the class is prepared to perform radiance estimation
// using k-nearest neighbors ('m_nb_photons') to define the bandwidth
// of the kernel.
//---------------------------------------------------------------------
Vector3 PhotonMapping::shade(Intersection &it0)const
{
	Vector3 L(0);
	Vector3 W(1, 1, 1);

	Intersection it(it0);

	//**********************************************************************
	// The following piece of code is included here for two reasons: first
	// it works as a 'hello world' code to check that everthing compiles 
	// just fine, and second, to illustrate some of the functions that you 
	// will need when doing the work. Goes without saying: remove the 
	// pieces of code that you won't be using.
	//
	unsigned int debug_mode = 8;

	switch (debug_mode)
	{
	case 1:
		// ----------------------------------------------------------------
		// Display Albedo Only
		L = it.intersected()->material()->get_albedo(it);
		break;
	case 2:
		// ----------------------------------------------------------------
		// Display Normal Buffer
		L = it.get_normal();
		break;
	case 3:
		// ----------------------------------------------------------------
		// Display whether the material is specular (or refractive) 
		L = Vector3(it.intersected()->material()->is_delta());
		break;

	case 4:
		// ----------------------------------------------------------------
		// Display incoming illumination from light(0)
		L = world->light(0).get_incoming_light(it.get_position());
		break;

	case 5:
		// ----------------------------------------------------------------
		// Display incoming direction from light(0)
		L = world->light(0).get_incoming_direction(it.get_position());
		break;

	case 6:
		// ----------------------------------------------------------------
		// Check Visibility from light(0)
		if (world->light(0).is_visible(it.get_position()))
			L = Vector3(1.);
		break;
	case 7:
	{
		// ----------------------------------------------------------------
		// Reflect and refract until a diffuse surface is found, then show its albedo...
		int nb_bounces = 0;
		// MAX_NB_BOUNCES defined in ./SmallRT/include/globals.h
		while (it.intersected()->material()->is_delta() && ++nb_bounces < MAX_NB_BOUNCES)
		{
			Ray r; float pdf;
			it.intersected()->material()->get_outgoing_sample_ray(it, r, pdf);
			W = W * it.intersected()->material()->get_albedo(it) / pdf;

			r.shift();
			world->first_intersection(r, it);
		}
		L = it.intersected()->material()->get_albedo(it);
		break;
	}
	case 8:
	{
		Vector3 Ld(0, 0, 0), Lc(0, 0, 0), Li(0, 0, 0);
		int nb_bounces = 0;
		// MAX_NB_BOUNCES defined in ./SmallRT/include/globals.h
		Ray ray;
		float pdf;
		//specular light
		while (it.intersected()->material()->is_delta() && ++nb_bounces < MAX_NB_BOUNCES)
		{
			ray = it.get_ray();
			it.intersected()->material()->get_outgoing_sample_ray(it, ray, pdf);
			W = W * it.intersected()->material()->get_albedo(it) / pdf;

			ray.shift();
			world->first_intersection(ray, it);
		}

		//direct
		LightSource* light = nullptr;
		if (!it.intersected()->material()->is_delta()) {
			for (int i = 0; i < world->light_source_list.size(); i++) {
				light = world->light_source_list[i];
				if (light->is_visible(it.get_position())) {
					Ld += it.intersected()->material()->get_albedo(it) / M_PI * (light->get_incoming_direction(it.get_position()).dot_abs(it.get_normal())) 
							* light->get_incoming_light(it.get_position());
				}
			}
		}
		Real k = 1.5;
		//caustic
		Real radius = 0;
		/*if (!it.intersected()->material()->is_delta()) {
			vector<const KDTree<Photon, 3>::Node*> nodes;
			radius = 0;
			Vector3 point = it.get_position();
			m_caustics_map.find(vector<Real>(point.data, point.data + sizeof(point.data) / sizeof(*point.data)), m_nb_photons, nodes, radius);
			for (int i = 0; i < nodes.size(); i++) {
				Lc += nodes[i]->data().flux * (it.intersected()->material()->get_albedo(it) / M_PI);
			}
			Lc = Lc / ((1 - 2 / (3 * k)) * M_PI * radius * radius);
		}*/

		//indirect
		/*if (!it.intersected()->material()->is_delta()) {
			vector<const KDTree<Photon, 3>::Node*> nodes;
			radius = 0;
			Vector3 point = it.get_position();
			m_global_map.find(vector<Real>(point.data, point.data + sizeof(point.data) / sizeof(*point.data)), m_nb_photons, nodes, radius);
			for (int i = 0; i < nodes.size(); i++) {
				Li += nodes[i]->data().flux * (it.intersected()->material()->get_albedo(it) / M_PI);
			}
			Li = Li / ((1 - 2 / (3 * k)) * M_PI * radius * radius);
		}*/

		L = Li + Lc + Ld;
		break;
	}
	case 9:
	{
		Vector3 Ld(0, 0, 0), Lc(0, 0, 0), Li(0, 0, 0);
		//specular
		int nb_bounces = 0;
		// MAX_NB_BOUNCES defined in ./SmallRT/include/globals.h
		while (it.intersected()->material()->is_delta() && ++nb_bounces < MAX_NB_BOUNCES)
		{
			Ray r = it.get_ray(); float pdf;
			it.intersected()->material()->get_outgoing_sample_ray(it, r, pdf);
			W = W * it.intersected()->material()->get_albedo(it) / pdf;

			r.shift();
			world->first_intersection(r, it);
		}

		Real k = 1.5, radius = 0;;
		//caustics
		if (!it.intersected()->material()->is_delta()) {
			vector<const KDTree<Photon, 3>::Node*> nodes;
			radius = 0;
			Vector3 point = it.get_position();
			m_caustics_map.find(vector<Real>(point.data, point.data + sizeof(point.data) / sizeof(*point.data)), 1, nodes, radius);
			for (int i = 0; i < nodes.size(); i++) {
				Lc += nodes[i]->data().flux * (it.intersected()->material()->get_albedo(it) / M_PI);
			}
			Lc = Lc / ((1 - 2 / (3 * k)) * M_PI * radius * radius);
		}

		//indirect
		if (!it.intersected()->material()->is_delta()) {
			vector<const KDTree<Photon, 3>::Node*> nodes;
			radius = 0;
			Vector3 point = it.get_position();
			m_global_map.find(vector<Real>(point.data, point.data + sizeof(point.data) / sizeof(*point.data)), 1, nodes, radius);
			for (int i = 0; i < nodes.size(); i++) {
				Li += nodes[i]->data().flux * (it.intersected()->material()->get_albedo(it) / M_PI);
			}
			Li = Li / ((1 - 2 / (3 * k)) * M_PI * radius * radius);
		}
		L = Li + Lc + Ld;
		break;
	}
	}
	// End of exampled code
	//**********************************************************************
/*
	if (debug_mode == 8) {
		Vector3 Ld(0, 0, 0), Lc(0, 0, 0), Li(0, 0, 0);

		//Luz especular
		int nb_bounces = 0;
		// MAX_NB_BOUNCES defined in ./SmallRT/include/globals.h
		while (it.intersected()->material()->is_delta() && ++nb_bounces < MAX_NB_BOUNCES)
		{
			Ray r = it.get_ray(); float pdf;
			it.intersected()->material()->get_outgoing_sample_ray(it, r, pdf);
			W = W * it.intersected()->material()->get_albedo(it) / pdf;

			r.shift();
			world->first_intersection(r, it);
		}



		//Luz directa
		if (!it.intersected()->material()->is_delta()) {
			for (int i = 0; i < world->light_source_list.size(); i++) {
				LightSource * l = world->light_source_list[i];
				if (l->is_visible(it.get_position())) {
					Ld += it.intersected()->material()->get_albedo(it) / M_PI * (l->get_incoming_direction(it.get_position()).dot_abs(it.get_normal())) * l->get_incoming_light(it.get_position());
				}
			}
		}
		Real k = 1.5;
		//Luz causticas
		if (!it.intersected()->material()->is_delta()) {
			vector<const KDTree<Photon, 3>::Node *> nodes;
			Real radius = 0;
			Vector3 point = it.get_position();
			m_caustics_map.find(vector<Real>(point.data, point.data + sizeof(point.data) / sizeof(*point.data)), m_nb_photons, nodes, radius);
			for (int i = 0; i < nodes.size(); i++) {
				Lc += nodes[i]->data().flux*(it.intersected()->material()->get_albedo(it) / M_PI);
			}
			Lc = Lc / ((1 - 2 / (3 * k))*M_PI*radius*radius);
		}

		//Luz indirecta
		if (!it.intersected()->material()->is_delta()) {
			vector<const KDTree<Photon, 3>::Node *> nodes;
			Real radius = 0;
			Vector3 point = it.get_position();
			m_global_map.find(vector<Real>(point.data, point.data + sizeof(point.data) / sizeof(*point.data)), m_nb_photons, nodes, radius);
			for (int i = 0; i < nodes.size(); i++) {
				Li += nodes[i]->data().flux*(it.intersected()->material()->get_albedo(it) / M_PI);
			}
			Li = Li / ((1 - 2 / (3 * k))*M_PI*radius*radius);
		}

		L = Li + Lc + Ld;
	}
	// muestra el mapa de fotones
	else if (debug_mode == 9) {
		Vector3 Ld(0, 0, 0), Lc(0, 0, 0), Li(0, 0, 0);


		//Luz especular
		int nb_bounces = 0;
		// MAX_NB_BOUNCES defined in ./SmallRT/include/globals.h
		while (it.intersected()->material()->is_delta() && ++nb_bounces < MAX_NB_BOUNCES)
		{
			Ray r = it.get_ray(); float pdf;
			it.intersected()->material()->get_outgoing_sample_ray(it, r, pdf);
			W = W * it.intersected()->material()->get_albedo(it) / pdf;

			r.shift();
			world->first_intersection(r, it);
		}


		Real k = 1.5;
		//Luz causticas
		if (!it.intersected()->material()->is_delta()) {
			vector<const KDTree<Photon, 3>::Node *> nodes;
			Real radius = 0;
			Vector3 point = it.get_position();
			m_caustics_map.find(vector<Real>(point.data, point.data + sizeof(point.data) / sizeof(*point.data)), 1, nodes, radius);
			for (int i = 0; i < nodes.size(); i++) {
				Lc += nodes[i]->data().flux*(it.intersected()->material()->get_albedo(it) / M_PI);
			}
			Lc = Lc / ((1 - 2 / (3 * k))*M_PI*radius*radius);
		}

		//Luz indirecta
		if (!it.intersected()->material()->is_delta()) {
			vector<const KDTree<Photon, 3>::Node *> nodes;
			Real radius = 0;
			Vector3 point = it.get_position();
			m_global_map.find(vector<Real>(point.data, point.data + sizeof(point.data) / sizeof(*point.data)), 1, nodes, radius);
			for (int i = 0; i < nodes.size(); i++) {
				Li += nodes[i]->data().flux*(it.intersected()->material()->get_albedo(it) / M_PI);
			}
			Li = Li / ((1 - 2 / (3 * k))*M_PI*radius*radius);
		}

		L = Li + Lc + Ld;
	}
	*/
	return L * W;
}