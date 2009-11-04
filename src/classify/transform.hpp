#ifndef LISA_TRANSFORM_IMPL_HPP
#define LISA_TRANSFORM_IMPL_HPP

//@{
/** Class performing identity transform, i.e. when source and target are equal.
 *  For most pairs it does nothing except for those just increasing problem size by a constant.
 *  
 *  @author Jan Tusch
 *  @version 3.0pre1
 */
/// Atomic Transform class for transforming equal signatures.
class Identity_AtomicTransform : public virtual Lisa_AtomicTransform {
 public:
  /// actual transform function
  virtual bool transform(int g ,int s,int t,
			 TransformObjects& to,
			 TransformDir dir);
};

static Lisa_AtomicTransform* IdentityAtomic = new Identity_AtomicTransform();
static Lisa_Transform* IdentityTransform = new Lisa_Transform(IdentityAtomic,IdentityAtomic,0);

//@}


//another transform

//@{
/** Class performing objective function transform from SumCi to SumWiCi.
 *  
 *  @author Jan Tusch
 *  @version 3.0pre1
 */

/// Atomic Transform class for transforming SumCi to SumWiCi.
class SumWiCi_AtomicTransform : public virtual Lisa_AtomicTransform {
 public:
  /// actual transform function
  virtual bool transform(int g ,int s,int t,
			 TransformObjects& to,
			 TransformDir dir);
};

static Lisa_AtomicTransform* SumWiCi_Atomic = new SumWiCi_AtomicTransform();
static Lisa_Transform* SumWiCi_Transform = new Lisa_Transform(SumWiCi_Atomic,SumWiCi_Atomic,0);

//@}

#endif
