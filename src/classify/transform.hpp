#ifndef LISA_TRANSFORM_IMPL_HPP
#define LISA_TRANSFORM_IMPL_HPP

/** Class performing identity transform, i.e. when source and target are equal.
				For most pairs it does nothing except for those just increasing problem size by a constant.
*/
class Identity_AtomicTransform : public virtual Lisa_AtomicTransform {
 public:
		/// actual transform function
		virtual bool transform(int g ,int s,int t,
																									TransformObjects& to,
																									TransformDir dir);
};

static Lisa_AtomicTransform* IdentityAtomic = new Identity_AtomicTransform();
static Lisa_Transform* IdentityTransform = new Lisa_Transform(IdentityAtomic,IdentityAtomic,0);




#endif
