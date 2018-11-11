#include "monte_carlo_specular_diffuse_group.h"

manta::MonteCarloSpecularDiffuseGroup::MonteCarloSpecularDiffuseGroup() {
}

manta::MonteCarloSpecularDiffuseGroup::~MonteCarloSpecularDiffuseGroup() {
}

void manta::MonteCarloSpecularDiffuseGroup::createAllEmitters() {
	int emitterCount = 0;
	if (m_diffuseEnabled) emitterCount++;
	if (m_specularEnabled) emitterCount++;

	initializeEmitters(emitterCount);
	if (m_diffuseEnabled)
	{
		m_diffuseEmitter = createEmitter<SimpleMonteCarloEmitter>();
	}
	else
	{
		m_diffuseEmitter = NULL;
	}

	if (m_specularEnabled) m_specularEmitter = createEmitter<SimpleRayEmitter>();
	else m_specularEmitter = NULL;

	// TODO: remove this hack
	m_rayEmitters[0] = (RayEmitter *)m_diffuseEmitter;
	if (m_specularEnabled && m_diffuseEnabled) m_rayEmitters[1] = (RayEmitter *)m_specularEmitter;
	else if (m_specularEnabled) m_rayEmitters[0] = (RayEmitter *)m_specularEmitter;
}