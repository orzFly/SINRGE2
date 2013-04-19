#include "RbExport.h"
#include "RbBitmap.h"
#include "RbViewport.h"
#include "RbRect.h"
#include "RbParticleSystem.h"
#include "sin_app.h"

VALUE rb_cParticleSystem;

RbParticleSystem::RbParticleSystem()
	: m_node(0)

	, fAge(-2.0f)
	, fEmissionResidue(0.0f)

	, fTx(0), fTy(0)
	, fScale(1.0f)

	, nParticlesAlive(0)
	, bUpdateBoundingBox(false)

	, m_ref_bitmap_modify_count(0)
	, m_bitmap_ptr(0)
	, mlast(0)
{
	memset(&info, 0, sizeof(info));
	vecLocation.x = vecPrevLocation.x = 0.0f;
	vecLocation.y = vecPrevLocation.y = 0.0f;
	rectBoundingBox.Clear();
	memset(particles, 0, sizeof(particles));

	m_pSpr = new hgeSprite(0, 0, 0, 0, 0);
	m_pSpr->SetBlendMode(BLEND_COLORMUL);
	m_pSpr->SetTextureRect(0, 0, 32, 32);
	m_pSpr->SetHotSpot(16, 16);
}

RbParticleSystem::~RbParticleSystem()
{
	SAFE_DELETE(m_pSpr);
	RbRenderTree::DestroyNode(&m_node);
}

void RbParticleSystem::InitLibrary()
{
	rb_cParticleSystem = rb_define_class_under(rb_mSin, "ParticleSystem", rb_cObject);

	// special method
	rb_define_alloc_func(rb_cParticleSystem, ObjAllocate<RbParticleSystem>);
	rb_define_method(rb_cParticleSystem, "initialize",		(RbFunc)dm_initialize,			-1);

	// instance method
	rb_define_method(rb_cParticleSystem, "dispose",			(RbFunc)dm_dispose,				0);
	rb_define_method(rb_cParticleSystem, "disposed?",		(RbFunc)dm_is_disposed,			0);

	rb_define_method(rb_cParticleSystem, "update",			(RbFunc)dm_update,				0);
	rb_define_method(rb_cParticleSystem, "fire_at",			(RbFunc)dm_fire_at,				2);
	rb_define_method(rb_cParticleSystem, "fire",			(RbFunc)dm_fire,				0);
	rb_define_method(rb_cParticleSystem, "stop",			(RbFunc)dm_stop,				-1);
	rb_define_method(rb_cParticleSystem, "move_to",			(RbFunc)dm_move_to,				-1);

	// object attribute
	rb_define_method(rb_cParticleSystem, "viewport",		(RbFunc)dm_get_viewport,		0);
	rb_define_method(rb_cParticleSystem, "viewport=",		(RbFunc)dm_set_viewport,		1);

	rb_define_method(rb_cParticleSystem, "visible",			(RbFunc)dm_get_visible,			0);
	rb_define_method(rb_cParticleSystem, "visible=",		(RbFunc)dm_set_visible,			1);
	rb_define_method(rb_cParticleSystem, "z",				(RbFunc)dm_get_z,				0);
	rb_define_method(rb_cParticleSystem, "z=",				(RbFunc)dm_set_z,				1);

	rb_define_method(rb_cParticleSystem, "bitmap",			(RbFunc)dm_get_bitmap,			0);
	rb_define_method(rb_cParticleSystem, "bitmap=",			(RbFunc)dm_set_bitmap,			1);

	// supplement
 	rb_define_method(rb_cParticleSystem, "to_s",			(RbFunc)dm_to_string,			0);
}

void RbParticleSystem::mark()
{
	if (m_viewport_ptr) m_viewport_ptr->MarkObject();
	if (m_bitmap_ptr)	m_bitmap_ptr->MarkObject();
}

VALUE RbParticleSystem::initialize(int argc, VALUE *argv, VALUE obj)
{
	VALUE filename, viewport;

	//	检查参数
	rb_scan_args(argc, argv, "11", &filename, &viewport);

	if (!NIL_P(viewport))
	{
		SafeViewportValue(viewport);
		m_viewport_ptr = GetObjectPtr<RbViewport>(viewport);
	}
	
	//	创建并添加渲染结点
	m_node = RbRenderTree::AllocNode(RenderProc, obj, 0, 0, viewport);
	RbRenderTree::InsertNode(m_node);

	m_visible = Qtrue;
	
	SafeStringValue(filename);

	HGE* hge = GetAppPtr()->GetHgePtr();

	void* psi = hge->Resource_Load(Kconv::UTF8ToUnicode(RSTRING_PTR(filename)));
	if (!psi) rb_raise(rb_eSinError, "Failed to load psi: `%s'.", Kconv::UTF8ToAnsi(RSTRING_PTR(filename)));
	memcpy(&info, psi, sizeof(hgeParticleSystemInfo));
	hge->Resource_Free(psi);

	info.sprite = m_pSpr;

	m_disposed = false;
	return obj;
}

void RbParticleSystem::check_raise()
{
	if (m_disposed)
		rb_raise(rb_eSinError, "disposed particle system");
}

VALUE RbParticleSystem::dispose()
{
	if (m_disposed)
		return Qnil;
	
	fAge=-2.0f;
	nParticlesAlive=0;
	rectBoundingBox.Clear();

	m_pSpr = 0;
	m_bitmap_ptr = 0;

	m_disposed = true;
	return Qnil;
}

VALUE RbParticleSystem::is_disposed()
{
	return C2RbBool(m_disposed);
}

VALUE RbParticleSystem::update()
{
	check_raise();

	float fDeltaTime = (GetAppPtr()->GetTick() - mlast) / 1000.0;
	mlast = GetAppPtr()->GetTick();
	HGE* hge = GetAppPtr()->GetHgePtr();

	int i;
	float ang;
	hgeParticle *par;
	hgeVector vecAccel, vecAccel2;

	if(fAge >= 0)
	{
		fAge += fDeltaTime;
		if(fAge >= info.fLifetime) fAge = -2.0f;
	}

	// update all alive particles

	if(bUpdateBoundingBox) rectBoundingBox.Clear();
	par=particles;

	for(i=0; i<nParticlesAlive; i++)
	{
		par->fAge += fDeltaTime;
		if(par->fAge >= par->fTerminalAge)
		{
			nParticlesAlive--;
			memcpy(par, &particles[nParticlesAlive], sizeof(hgeParticle));
			i--;
			continue;
		}

		vecAccel = par->vecLocation-vecLocation;
		vecAccel.Normalize();
		vecAccel2 = vecAccel;
		vecAccel *= par->fRadialAccel;

		// vecAccel2.Rotate(M_PI_2);
		// the following is faster
		ang = vecAccel2.x;
		vecAccel2.x = -vecAccel2.y;
		vecAccel2.y = ang;

		vecAccel2 *= par->fTangentialAccel;
		par->vecVelocity += (vecAccel+vecAccel2)*fDeltaTime;
		par->vecVelocity.y += par->fGravity*fDeltaTime;

		par->vecLocation += par->vecVelocity*fDeltaTime;

		par->fSpin += par->fSpinDelta*fDeltaTime;
		par->fSize += par->fSizeDelta*fDeltaTime;
		par->colColor += par->colColorDelta*fDeltaTime;

		if(bUpdateBoundingBox) rectBoundingBox.Encapsulate(par->vecLocation.x, par->vecLocation.y);

		par++;
	}

	// generate new particles

	if(fAge != -2.0f)
	{
		float fParticlesNeeded = info.nEmission*fDeltaTime + fEmissionResidue;
		int nParticlesCreated = (unsigned int)fParticlesNeeded;
		fEmissionResidue=fParticlesNeeded-nParticlesCreated;

		par=&particles[nParticlesAlive];

		for(i=0; i<nParticlesCreated; i++)
		{
			if(nParticlesAlive>=MAX_PARTICLES) break;

			par->fAge = 0.0f;
			par->fTerminalAge = hge->Random_Float(info.fParticleLifeMin, info.fParticleLifeMax);

			par->vecLocation = vecPrevLocation+(vecLocation-vecPrevLocation)*hge->Random_Float(0.0f, 1.0f);
			par->vecLocation.x += hge->Random_Float(-2.0f, 2.0f);
			par->vecLocation.y += hge->Random_Float(-2.0f, 2.0f);

			ang=info.fDirection-M_PI_2+hge->Random_Float(0,info.fSpread)-info.fSpread/2.0f;
			if(info.bRelative) ang += (vecPrevLocation-vecLocation).Angle()+M_PI_2;
			par->vecVelocity.x = cosf(ang);
			par->vecVelocity.y = sinf(ang);
			par->vecVelocity *= hge->Random_Float(info.fSpeedMin, info.fSpeedMax);

			par->fGravity = hge->Random_Float(info.fGravityMin, info.fGravityMax);
			par->fRadialAccel = hge->Random_Float(info.fRadialAccelMin, info.fRadialAccelMax);
			par->fTangentialAccel = hge->Random_Float(info.fTangentialAccelMin, info.fTangentialAccelMax);

			par->fSize = hge->Random_Float(info.fSizeStart, info.fSizeStart+(info.fSizeEnd-info.fSizeStart)*info.fSizeVar);
			par->fSizeDelta = (info.fSizeEnd-par->fSize) / par->fTerminalAge;

			par->fSpin = hge->Random_Float(info.fSpinStart, info.fSpinStart+(info.fSpinEnd-info.fSpinStart)*info.fSpinVar);
			par->fSpinDelta = (info.fSpinEnd-par->fSpin) / par->fTerminalAge;

			par->colColor.r = hge->Random_Float(info.colColorStart.r, info.colColorStart.r+(info.colColorEnd.r-info.colColorStart.r)*info.fColorVar);
			par->colColor.g = hge->Random_Float(info.colColorStart.g, info.colColorStart.g+(info.colColorEnd.g-info.colColorStart.g)*info.fColorVar);
			par->colColor.b = hge->Random_Float(info.colColorStart.b, info.colColorStart.b+(info.colColorEnd.b-info.colColorStart.b)*info.fColorVar);
			par->colColor.a = hge->Random_Float(info.colColorStart.a, info.colColorStart.a+(info.colColorEnd.a-info.colColorStart.a)*info.fAlphaVar);

			par->colColorDelta.r = (info.colColorEnd.r-par->colColor.r) / par->fTerminalAge;
			par->colColorDelta.g = (info.colColorEnd.g-par->colColor.g) / par->fTerminalAge;
			par->colColorDelta.b = (info.colColorEnd.b-par->colColor.b) / par->fTerminalAge;
			par->colColorDelta.a = (info.colColorEnd.a-par->colColor.a) / par->fTerminalAge;

			if(bUpdateBoundingBox) rectBoundingBox.Encapsulate(par->vecLocation.x, par->vecLocation.y);

			nParticlesAlive++;
			par++;
		}
	}

	vecPrevLocation=vecLocation;

	return Qnil;
}

VALUE RbParticleSystem::fire()
{
	check_raise();

	if(info.fLifetime==-1.0f) fAge=-1.0f;
	else fAge=0.0f;

	return Qnil;
}

VALUE RbParticleSystem::fire_at(VALUE vx, VALUE vy)
{
	check_raise();
	SafeNumericValue(vx);
	SafeNumericValue(vy);
	
	float x = NUM2FLOAT(vx), y = NUM2FLOAT(vy);
	
	vecPrevLocation.x=x;
	vecPrevLocation.y=y;
	vecLocation.x=x;
	vecLocation.y=y;
	if(info.fLifetime==-1.0f) fAge=-1.0f;
	else fAge=0.0f;

	return Qnil;
}

VALUE RbParticleSystem::stop(int argc, VALUE *argv, VALUE obj)
{
	check_raise();
	
	fAge=-2.0f;
	if (RTEST(argv[0]))
	{
		nParticlesAlive=0;
		rectBoundingBox.Clear();
	}
	return Qnil;
}

VALUE RbParticleSystem::move_to(int argc, VALUE *argv, VALUE obj)
{
	check_raise();
	VALUE vx, vy, move;
	rb_scan_args(argc, argv, "21", &vx, &vy, &move);
	SafeNumericValue(vx);
	SafeNumericValue(vy);

	int i;
	float dx,dy;
	float x = NUM2FLOAT(vx), y = NUM2FLOAT(vy);
	
	if(RTEST(move))
	{
		dx=x-vecLocation.x;
		dy=y-vecLocation.y;

		for(i=0;i<nParticlesAlive;i++)
		{
			particles[i].vecLocation.x += dx;
			particles[i].vecLocation.y += dy;
		}

		vecPrevLocation.x=vecPrevLocation.x + dx;
		vecPrevLocation.y=vecPrevLocation.y + dy;
	}
	else
	{
		if(fAge==-2.0) { vecPrevLocation.x=x; vecPrevLocation.y=y; }
		else { vecPrevLocation.x=vecLocation.x;	vecPrevLocation.y=vecLocation.y; }
	}

	vecLocation.x=x;
	vecLocation.y=y;

	return Qnil;
}

/*
 *	渲染（描绘）
 */
void RbParticleSystem::render(u32 id)
{
	//	有效性检查
	if (!m_bitmap_ptr)
		return;

	if (m_bitmap_ptr->IsDisposed())
		return;

	// render the sprite to the screen
	float x = 0.0f, y = 0.0f;

	if (m_viewport_ptr)
	{
		const RbRect* rect_ptr = m_viewport_ptr->GetRectPtr();
		x = rect_ptr->x - m_viewport_ptr->m_ox;
		y = rect_ptr->y - m_viewport_ptr->m_oy;
	}
	Render(x, y);
}

void RbParticleSystem::Render(float offset_x, float offset_y)
{
	int i;
	DWORD col;
	hgeParticle *par=particles;

	col=info.sprite->GetColor();

	for(i=0; i<nParticlesAlive; i++)
	{
		if(info.colColorStart.r < 0)
			info.sprite->SetColor(SETA(info.sprite->GetColor(),par->colColor.a*255));
		else
			info.sprite->SetColor(par->colColor.GetHWColor());
		info.sprite->RenderEx(par->vecLocation.x*fScale+fTx+offset_x, par->vecLocation.y*fScale+fTy+offset_y, par->fSpin*par->fAge, par->fSize*fScale);
		par++;
	}

	info.sprite->SetColor(col);
}

VALUE RbParticleSystem::get_bitmap()
{
	return ReturnObject(m_bitmap_ptr);
}

VALUE RbParticleSystem::set_bitmap(VALUE bitmap)
{
	if (m_disposed)	
		return Qnil;

	// 设置位图修改计数值为-1
	m_ref_bitmap_modify_count = -1;

	if (NIL_P(bitmap))
		m_bitmap_ptr = 0;
	else
	{
		SafeBitmapValue(bitmap);
		m_bitmap_ptr = GetObjectPtr<RbBitmap>(bitmap);
		m_pSpr->SetTexture(m_bitmap_ptr->GetBitmapPtr()->quad.tex);
	}
	return Qnil;
}

VALUE RbParticleSystem::set_z(VALUE z)
{
	SafeFixnumValue(z);

	if (m_z != FIX2INT(z))
	{
		m_z = FIX2INT(z);
		m_node->z = m_z;
		RbRenderTree::InsertNode(RbRenderTree::DeleteNode(m_node));
	}

	return Qnil;
}

VALUE RbParticleSystem::set_viewport(VALUE viewport)
{
	if (NIL_P(viewport))
	{
		m_viewport_ptr = 0;
	}
	else
	{
		SafeViewportValue(viewport);
		m_viewport_ptr = GetObjectPtr<RbViewport>(viewport);
	}

	RbRenderTree::DeleteNode(m_node);
	m_node->viewport = viewport;
	RbRenderTree::InsertNode(m_node);

	return Qnil;
}

/*
 *	以下定义ruby方法
 */
imp_method(RbParticleSystem, dispose)
imp_method(RbParticleSystem, is_disposed)

imp_method(RbParticleSystem, update)
imp_method02(RbParticleSystem, fire_at)
imp_method(RbParticleSystem, fire)
imp_method_vargs(RbParticleSystem, stop)
imp_method_vargs(RbParticleSystem, move_to)

imp_attr_accessor(RbParticleSystem, bitmap)