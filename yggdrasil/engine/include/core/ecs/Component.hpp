#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <AntTweakBar.h>

namespace ygg
{
	// A integral type that is used to define the "type" of the component at run time
	typedef int ComponentType;

	// A struct that defines a component that can be attached to an entity
	struct Component
	{
	public:
        virtual ~Component() {
        }

        virtual void display(TwBar* bar) {
        }

	protected:
		Component() {};

	private:
		friend class ComponentManager;
	};
}

#endif /* __COMPONENT_H__ */
