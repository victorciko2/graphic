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
	float total_p = 0;
	for (int i = 0; i < world->light_source_list.size(); i++) {
		total_p += world->light_source_list[i]->get_intensities().length();
	}
	list<Photon> global, caustic;
	for (int i = 0; i < world->light_source_list.size(); i++) {
		LightSource * luz = world->light_source_list[i];
		int nSamples = m_nb_global_photons * (luz->get_intensities().length() / total_p);

		for(int i = 0; i < nSamples; i++){
			theta = 2 * M_PI * distribution(gen);
			phi = acos(1 - 2 * distribution(gen));
			direction = Vector3(sin(phi) * cos(theta), sin(phi) * sin(theta), cos(phi));
			direction.normalize();
			trace_ray(Ray(luz->get_position(), direction), 4 * M_PI * luz->get_intensities() / nSamples,
					global, caustic, false, false);
		}
	}

	bool balanced = true;
	std::list<Photon>::const_iterator iterator;
	for (iterator = global.begin(); iterator != global.end(); ++iterator) {
		Photon p = *iterator;
		std::vector<Real> r(p.position.data, p.position.data + 3);
		m_global_map.store(r, p);
		balanced = false;
	}
	if (!balanced) {
		m_global_map.balance();
		balanced = true;
	}

	balanced = true;
	for (iterator = caustic.begin(); iterator != caustic.end(); ++iterator) {
		Photon p = *iterator;
		std::vector<Real> r(p.position.data, p.position.data + 3);
		m_caustics_map.store(r, p);
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

	}
	// End of exampled code
	//**********************************************************************

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
					Ld += it.intersected()->material()->get_albedo(it) / _Pi * (l->get_incoming_direction(it.get_position()).dot_abs(it.get_normal())) * l->get_incoming_light(it.get_position());
				}
			}
		}
		Real k = 1.5;
		//Luz causticas
		if (!it.intersected()->material()->is_delta()) {
			vector<const KDTree<Photon, 3>::Node *> nodos;
			Real radio = 0;
			Vector3 point = it.get_position();
			m_caustics_map.find(vector<Real>(point.data, point.data + sizeof(point.data) / sizeof(*point.data)), m_nb_photons, nodos, radio);
			for (int i = 0; i < nodos.size(); i++) {
				Lc += nodos[i]->data().flux*(it.intersected()->material()->get_albedo(it) / _Pi);
			}
			Lc = Lc / ((1 - 2 / (3 * k))*_Pi*radio*radio);
		}

		//Luz indirecta
		if (!it.intersected()->material()->is_delta()) {
			vector<const KDTree<Photon, 3>::Node *> nodos;
			Real radio = 0;
			Vector3 point = it.get_position();
			m_global_map.find(vector<Real>(point.data, point.data + sizeof(point.data) / sizeof(*point.data)), m_nb_photons, nodos, radio);
			for (int i = 0; i < nodos.size(); i++) {
				Li += nodos[i]->data().flux*(it.intersected()->material()->get_albedo(it) / _Pi);
			}
			Li = Li / ((1 - 2 / (3 * k))*_Pi*radio*radio);
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
			vector<const KDTree<Photon, 3>::Node *> nodos;
			Real radio = 0;
			Vector3 point = it.get_position();
			m_caustics_map.find(vector<Real>(point.data, point.data + sizeof(point.data) / sizeof(*point.data)), 1, nodos, radio);
			for (int i = 0; i < nodos.size(); i++) {
				Lc += nodos[i]->data().flux*(it.intersected()->material()->get_albedo(it) / _Pi);
			}
			Lc = Lc / ((1 - 2 / (3 * k))*_Pi*radio*radio);
		}

		//Luz indirecta
		if (!it.intersected()->material()->is_delta()) {
			vector<const KDTree<Photon, 3>::Node *> nodos;
			Real radio = 0;
			Vector3 point = it.get_position();
			m_global_map.find(vector<Real>(point.data, point.data + sizeof(point.data) / sizeof(*point.data)), 1, nodos, radio);
			for (int i = 0; i < nodos.size(); i++) {
				Li += nodos[i]->data().flux*(it.intersected()->material()->get_albedo(it) / _Pi);
			}
			Li = Li / ((1 - 2 / (3 * k))*_Pi*radio*radio);
		}

		L = Li + Lc + Ld;
	}

	return L * W;
}