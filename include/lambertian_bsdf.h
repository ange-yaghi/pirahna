#ifndef LAMBERTIAN_BSDF_H
#define LAMBERTIAN_BSDF_H

#include <bsdf.h>

namespace manta {

	class LambertianBSDF : public BSDF {
	public:
		LambertianBSDF();
		~LambertianBSDF();

		virtual math::Vector generateMicrosurfaceNormal(const BSDFInput &bsdfInput) const;
		virtual math::real generateWeight(const BSDFInput &bsdfInput, const math::Vector &m, const math::Vector &o) const;
		virtual math::real bidirectionalShadowMasking(const BSDFInput &bsdfInput, const math::Vector &o, const math::Vector &m) const;

	protected:
		virtual math::real g1(const BSDFInput &bsdfInput, const math::Vector &v, const math::Vector &m) const;
	};

} /* namespace manta */

#endif /* LAMBERTIAN_BSDF_H */

